#ifndef BOARDIMAGEPROCSSOR_H
#define BOARDIMAGEPROCSSOR_H

#include "Net/downloader.h"
#include "Dal/image/flickrimageprovider.h"

#include <QtConcurrent>
#include <QFuture>
#include <QBuffer>
#include <QPoint>
#include <any>

namespace Dal::Image {

class FutureEvents {
public:
    virtual void finished() = 0;
};

class FutureWatcherBase : public QObject, public FutureEvents {
    Q_OBJECT

public:
    explicit FutureWatcherBase(QObject *parent = nullptr)
        : QObject(parent)
        , waiterThread_(nullptr)
    {}

    template<class FResult>
    void startWatching(const std::future<FResult> &futureToWait)
    {
        if(! futureToWait.valid()){
            emit finished();
            return;
        }
        waiterThread_ = QThread::create([&futureToWait] {
            futureToWait.wait();
        });
        assert(waiterThread_);
        connect(waiterThread_, &QThread::finished, this, &FutureWatcherBase::finished, Qt::QueuedConnection);
        waiterThread_->start();
    }

    //virtual std::any future() const = 0;

    ~FutureWatcherBase()
    {
        if (waiterThread_)
            waiterThread_->wait();
    }

signals:
    void finished() override;

private:
    QThread *waiterThread_;
};

template<class FResult>
class FutureWatcher : public FutureWatcherBase {
public:
    explicit FutureWatcher(QObject *parent = nullptr)
        : FutureWatcherBase(parent)
    {}

    void setFuture(const std::future<FResult> &future)
    {
        if (future_ == future)
            return;
        startWatching(future_);
        future_ = future;
    }

    std::future<FResult> future() const
    {
        return future_;
    }

private:
    std::future<FResult> future_;
};

class BoardImageController : public QObject {
    Q_OBJECT

    using IImageProviderPtr = std::unique_ptr<IRundomImageProvider>;


public:
    BoardImageController(QObject *parent = nullptr)
        : QObject(parent)
    {
        connect(&imgWatcher_, &QFutureWatcher<std::vector<QByteArray>>::finished,
                this, &BoardImageController::sigBoardImagesReady);
    }

    ~BoardImageController() { DEBUG("~BoardImageController") }

    QFuture<std::vector<QByteArray>> getBoardImagesAsync(const QPoint &dimensions)
    {
        const auto imgFuture = QtConcurrent::run(&workerPool_, [this, dimensions] {
            const auto downloader = std::make_shared<Net::Downloader>();
            const IImageProviderPtr imgProvider_ = std::make_unique<FlickrImageProvider>(downloader);

            DEBUG("getBoardImagesAsync" << QThread::currentThreadId());

            QImage fullImage;


            try {
                fullImage = imgProvider_->getRundomImage();
            } catch (Net::NetworkError &ex) {
            }

            int w = fullImage.width();
            int h = fullImage.height();
            int partW = w / dimensions.x();
            int partH = h / dimensions.y();

            int wEnd = w - partW / 2;
            int hEnd = h - partH / 2;

            size_t partsSize = static_cast<size_t>(dimensions.x() * dimensions.y());
            std::vector<QFuture<QByteArray>> partsFutures;
            partsFutures.reserve(partsSize);

            for (int yi = 0; yi < hEnd; yi += partH) {
                for (int xi = 0; xi < wEnd; xi += partW) {
                    partsFutures.emplace_back(
                        partitionImage(fullImage, QRect(xi, yi, partW, partH)));
                }
            }

            std::vector<QByteArray> imgParts;
            imgParts.reserve(partsSize);

            for (auto &&f : partsFutures) {
                imgParts.emplace_back(f.result());
            }

            return imgParts;
        });

        imgWatcher_.setFuture(imgFuture);
        return imgFuture;
    }

    std::future<std::vector<QByteArray>> getImages(const QPoint &dimensions)
    {

        watcher_.setFuture( std::async(std::launch::async, [this, dimensions] {
            const auto downloader = std::make_shared<Net::Downloader>();
            const IImageProviderPtr imgProvider_ = std::make_unique<FlickrImageProvider>(downloader);

            DEBUG("getBoardImagesAsync" << QThread::currentThreadId());

            QImage fullImage;

            fullImage = imgProvider_->getRundomImage();

            int w = fullImage.width();
            int h = fullImage.height();
            int partW = w / dimensions.x();
            int partH = h / dimensions.y();

            int wEnd = w - partW / 2;
            int hEnd = h - partH / 2;

            size_t partsSize = static_cast<size_t>(dimensions.x() * dimensions.y());
            std::vector<QFuture<QByteArray>> partsFutures;
            partsFutures.reserve(partsSize);

            for (int yi = 0; yi < hEnd; yi += partH) {
                for (int xi = 0; xi < wEnd; xi += partW) {
                    partsFutures.emplace_back(
                        partitionImage(fullImage, QRect(xi, yi, partW, partH)));
                }
            }

            std::vector<QByteArray> imgParts;
            imgParts.reserve(partsSize);

            for (auto &&f : partsFutures) {
                imgParts.emplace_back(f.result());
            }

            return imgParts;
        }));
        return watcher_.future();
    }

signals:
    void sigBoardImagesReady();

private:
    QFuture<QByteArray> partitionImage(const QImage &img, const QRect rect)
    {
        return QtConcurrent::run(&workerPool_, [&img, rect] {
            QBuffer buffer;
            buffer.open(QIODevice::WriteOnly);
            img.copy(rect).save(&buffer, "JPEG");
            return buffer.data().toBase64().prepend("data:image/jpg;base64,");
        });
    }

private:
    FutureWatcher<std::vector<QByteArray>> watcher_;
    QFutureWatcher<std::vector<QByteArray>> imgWatcher_;
    QThreadPool workerPool_;
};


} // namespace Dal::Image
#endif // BOARDIMAGEPROCSSOR_H
