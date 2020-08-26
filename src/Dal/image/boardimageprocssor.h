#ifndef BOARDIMAGEPROCSSOR_H
#define BOARDIMAGEPROCSSOR_H

#include "Net/downloader.h"
#include "Dal/image/flickrimageprovider.h"

#include <QtConcurrent>
#include <QFuture>
#include <QBuffer>
#include <QPoint>

namespace Dal {
namespace Image {


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
        const auto imgFuture = QtConcurrent::run(&workerPool_, [this, dimensions]{
            const auto downloader = std::make_shared<Net::Downloader>();
            const IImageProviderPtr imgProvider_ = std::make_unique<FlickrImageProvider>(downloader);

            DEBUG("getBoardImagesAsync"<<QThread::currentThreadId());

            const QImage fullImage(imgProvider_->getRundomImage());

            int w = fullImage.width();
            int h = fullImage.height();
            int partW = w / dimensions.x();
            int partH = h / dimensions.y();

            int wEnd = w - partW/2;
            int hEnd = h - partH/2;

            size_t partsSize = static_cast<size_t>(dimensions.x() * dimensions.y());
            std::vector<QFuture<QByteArray>> partsFutures;
            partsFutures.reserve(partsSize);

              for (int yi = 0; yi < hEnd; yi += partH) {
                  for (int xi = 0; xi < wEnd; xi += partW) {
                      partsFutures.emplace_back(
                          PartitionImage(fullImage, QRect(xi, yi, partW, partH))
                      );
                  }
              }

              std::vector<QByteArray> imgParts;
              imgParts.reserve(partsSize);

              for (auto && f: partsFutures) {
                  imgParts.emplace_back(f.result());
              }

            return imgParts;
        });

        imgWatcher_.setFuture(imgFuture);
        return imgFuture;
    }

signals:
    void sigBoardImagesReady();

private:
    QFuture<QByteArray> PartitionImage(const QImage &img, const QRect rect)
    {
      return QtConcurrent::run(&workerPool_, [&img, rect]{
                QBuffer buffer;
                buffer.open(QIODevice::WriteOnly);
                img.copy(rect).save(&buffer, "JPEG");
                return buffer.data().toBase64().prepend("data:image/jpg;base64,");
      });
    }

private:
    QFutureWatcher<std::vector<QByteArray>> imgWatcher_;
    QThreadPool workerPool_;
};


}
}
#endif // BOARDIMAGEPROCSSOR_H
