#include "boardimagecontroller.h"
#include "main.h"

namespace Dal::Image {


BoardImageController::BoardImageController(QObject *parent)
    : QObject(parent)
{
    connect(&watcher_, &FutureWatcher<BoardImages>::sigResultReady,
            this, &BoardImageController::sigBoardImagesReady, Qt::QueuedConnection);
}

BoardImageController::~BoardImageController()
{
    DEBUG("~BoardImageController");
}

void BoardImageController::getBoardImagesAsync(const QPoint &dimensions)
{
    auto tmpFuture = std::async(
                std::launch::async, [dimensions]() -> BoardImages {
                    using namespace std::chrono_literals;

                    std::atomic_bool cancelationRequest = false;
                    auto downloader = std::make_shared<Net::Downloader>(cancelationRequest);
                    downloader->setTimeout(5s);

                    auto imgProvider = std::make_unique<FlickrImageProvider>(downloader);

                    const QImage fullImage(imgProvider->getRundomImage());

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

BoardImageController::BoardImages BoardImageController::getBoardImagesAcyncResult()
{
    return watcher_.getResult();
}

std::future<QByteArray> BoardImageController::partitionImages(const QImage &img, const QRect rect)
{
    return std::async([&img, rect] {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        img.copy(rect).save(&buffer, "JPEG");
        return buffer.data().toBase64().prepend("data:image/jpg;base64,");
    });
}


}
