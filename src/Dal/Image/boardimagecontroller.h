#ifndef BOARDIMAGEPROCSSOR_H
#define BOARDIMAGEPROCSSOR_H

#include "Net/downloader.h"
#include "Dal/Image/Providers/flickrimageprovider.h"
#include "Utils/StdFutureWatcher/src/futurewatcher.hpp"

#include <QtConcurrent>
#include <QFuture>
#include <QBuffer>
#include <QPoint>
#include <any>


namespace Dal::Image {


using namespace childcity::StdFutureWatcher;

class BoardImageController : public QObject {
    Q_OBJECT

    using IImageProviderPtr = std::unique_ptr<IRundomImageProvider>;
    using BoardImages = std::vector<QByteArray>;

public:
    BoardImageController(QObject *parent = nullptr)
        : QObject(parent)
    {
        connect(&watcher_, &FutureWatcher<BoardImages>::sigResultReady,
                this, &BoardImageController::sigBoardImagesReady, Qt::QueuedConnection);
    }

    ~BoardImageController()
    {
        DEBUG("~BoardImageController");
    }

    void getBoardImagesAsync(const QPoint &dimensions)
    {
        auto tmpFuture = std::async(
            std::launch::async, [dimensions]() -> BoardImages {
                const auto downloader = std::make_shared<Net::Downloader>();
                const IImageProviderPtr imgProvider_ = std::make_unique<FlickrImageProvider>(downloader);

                const QImage fullImage(imgProvider_->getRundomImage());

                const int w = fullImage.width();
                const int h = fullImage.height();
                const int partW = w / dimensions.x();
                const int partH = h / dimensions.y();

                const int wEnd = w - partW / 2;
                const int hEnd = h - partH / 2;

                const size_t partsSize = static_cast<size_t>(dimensions.x() * dimensions.y());

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

                return imgParts;
            });

        watcher_.setFuture(std::move(tmpFuture));
    }

    BoardImages getBoardImagesAcyncResult()
    {
        return watcher_.getResult();
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
    FutureWatcher<BoardImages> watcher_;
};


} // namespace Dal::Image
#endif // BOARDIMAGEPROCSSOR_H
