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

class IFutureEvents {
public:
    virtual void sigResultReady() = 0;
};

class FutureWatcherBase : public QObject, public IFutureEvents {
    Q_OBJECT

public:
    explicit FutureWatcherBase(QObject *parent = nullptr)
        : QObject(parent)
        , waiterThread_(nullptr)
    {}

    template<class FResult>
    void startWatching(const std::future<FResult> *futureToWait)
    {
        // don't support invalid future here
        assert(futureToWait->valid());

        waiterThread_ = QThread::create([futureToWait] {
            futureToWait->wait();
        });

        assert(waiterThread_);

        connect(waiterThread_, &QThread::finished, this, &FutureWatcherBase::sigResultReady, Qt::DirectConnection);
        connect(
            waiterThread_, &QThread::finished, waiterThread_, [this] {
                if (waiterThread_) {
                    waiterThread_->deleteLater();
                    waiterThread_ = nullptr;
                }
            },
            Qt::DirectConnection);

        waiterThread_->start();
    }

    ~FutureWatcherBase() override
    {
        DEBUG("~FutureWatcherBase ->")
        if (waiterThread_) {
            if (waiterThread_->isRunning()) {
                waiterThread_->disconnect(waiterThread_, &QThread::finished, nullptr, nullptr);
                //waiterThread_->wait();
                //waiterThread_->deleteLater();
                waiterThread_ = nullptr;
            }
        }
        DEBUG("~FutureWatcherBase <-")
    }

signals:
    void sigResultReady() override;

private:
    QPointer<QThread> waiterThread_;
};

template<class FResult>
class FutureWatcher : public FutureWatcherBase {
public:
    explicit FutureWatcher(QObject *parent = nullptr)
        : FutureWatcherBase(parent)
    {}

    ~FutureWatcher() override
    {
        DEBUG("~FutureWatcher");
    }

    void setFuture(std::future<FResult> future)
    {
        // future_ must be invalid. If future_ is valid, then this future has set before
        // FutureWatcher is watching only for one future
        assert(! future_.valid());

        future_ = std::move(future);

        startWatching<FResult>(&future_);
    }

    FResult getResult()
    {
        DEBUG("getResult")
        return future_.get();
    }

    std::future<FResult> const *future() const
    {
        return &future_;
    }

private:
    std::future<FResult> future_;
};

class BoardImageController : public QObject {
    Q_OBJECT

    using IImageProviderPtr = std::unique_ptr<IRundomImageProvider>;
    using BoardImages = std::vector<QByteArray>;

public:
    BoardImageController(QObject *parent = nullptr)
        : QObject(parent)
        , watcher_(new FutureWatcher<BoardImages>(this))
    {
        connect(watcher_, &FutureWatcher<BoardImages>::sigResultReady,
                this, &BoardImageController::sigBoardImagesReady, Qt::QueuedConnection);
    }

    ~BoardImageController()
    {
        DEBUG("~BoardImageController");
    }

    void getBoardImagesAsync(const QPoint &dimensions)
    {
        auto future = std::async(
            std::launch::async, [dimensions]() -> BoardImages {
                const auto downloader = std::make_shared<Net::Downloader>();
                const IImageProviderPtr imgProvider_ = std::make_unique<FlickrImageProvider>(downloader);

                DEBUG("getBoardImagesAsync" << QThread::currentThreadId());

                const QImage fullImage(imgProvider_->getRundomImage());

                const int w = fullImage.width();
                const int h = fullImage.height();
                const int partW = w / dimensions.x();
                const int partH = h / dimensions.y();

                const int wEnd = w - partW / 2;
                const int hEnd = h - partH / 2;

                const size_t partsSize = static_cast<size_t>(dimensions.x() * dimensions.y());
                DEBUG("partsSize"<<partsSize);

                std::vector<std::future<QByteArray>> partsFutures;
                BoardImages imgParts;
                partsFutures.reserve(partsSize);
                imgParts.reserve(partsSize);

                for (int yi = 0; yi < hEnd; yi += partH) {
                    for (int xi = 0; xi < wEnd; xi += partW) {
                        partsFutures.emplace_back(
                            partitionImages(fullImage, QRect(xi, yi, partW, partH)));
                    }
                }

                for (auto &&f : partsFutures) {
                    imgParts.emplace_back(f.get());
                }

                DEBUG("imgParts.size"<<imgParts.size());

                return imgParts;
            });

        watcher_->setFuture(std::move(future));
    }

    BoardImages getBoardImagesAcyncResult()
    {
        return watcher_->getResult();
    }

signals:
    void sigBoardImagesReady();

private:
    static std::future<QByteArray> partitionImages(const QImage &img, const QRect rect)
    {
        return std::async([&img, rect] {
            QBuffer buffer;
            buffer.open(QIODevice::WriteOnly);
            img.copy(rect).save(&buffer, "JPEG");
            return buffer.data().toBase64().prepend("data:image/jpg;base64,");
        });
    }

private:
    FutureWatcher<BoardImages> *watcher_;
};

} // namespace Dal::Image
#endif // BOARDIMAGEPROCSSOR_H
