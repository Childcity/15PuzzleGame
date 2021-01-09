#include "boardimagecontroller.h"
#include "main.h"

#include "Net/downloader.h"
#include "Providers/imageproviderfactory.h"

#include <QSize>
#include <QBuffer>
#include <QImage>


namespace Dal::Image {


BoardImageController::BoardImageController(QObject *parent) noexcept
    : QObject(parent)
{
    connect(&watcher_, &FutureWatcher<BoardImages>::sigResultReady,
            this, &BoardImageController::sigBoardImagesReady, Qt::QueuedConnection);
}

BoardImageController::~BoardImageController()
{
    DEBUG("~BoardImageController");
}

void BoardImageController::getBoardImagesAsync(const QSize &dimensions, ImageProviderType providerType)
{
    auto tmpFuture = std::async(
        std::launch::async, [dimensions, providerType]() -> BoardImages {
            using namespace std::chrono_literals;

            std::atomic_bool cancelationRequest = false;
            const auto downloader = std::make_shared<Net::Downloader>(cancelationRequest);
            downloader->setTimeout(5s);

            const auto imgProvider = ImageProviderFactory::GetImageProvider(providerType, downloader);

            const QImage fullImage = imgProvider->getRundomImage();

            const int w = fullImage.width();
            const int h = fullImage.height();
            const int partW = w / dimensions.width();
            const int partH = h / dimensions.height();

            const int wEnd = w - partW / 2;
            const int hEnd = h - partH / 2;

            const size_t partsSize = static_cast<size_t>(dimensions.width() * dimensions.height());

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


} // namespace Dal::Image
