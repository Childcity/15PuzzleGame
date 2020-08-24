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


class BoardImageProcessor {
  using IImageProviderPtr = std::unique_ptr<IRundomImageProvider>;

public:
  BoardImageProcessor()
  {
  }

  QFuture<std::vector<QByteArray>> getPartitions(const QPoint &dimensions)
  {
    return QtConcurrent::run([dimensions]{
      auto downloader = std::make_shared<Net::Downloader>();
      IImageProviderPtr imgProvider_ = std::make_unique<FlickrImageProvider>(downloader);


      const QImage fullImage(imgProvider_->getRundomImage());

      int w = fullImage.width();
      int h = fullImage.height();
      int partW = w / dimensions.x();
      int partH = h / dimensions.y();

      int wEnd = w - partW/2;
      int hEnd = h - partH/2;

      std::vector<QFuture<QByteArray>> partsFutures;
      partsFutures.reserve(dimensions.x() * dimensions.y());

        for (int yi = 0; yi < hEnd; yi += partH) {
          for (int xi = 0; xi < wEnd; xi += partW) {
            partsFutures.emplace_back(
                PartitionImage(fullImage, QRect(xi, yi, partW, partH))
            );
          }
        }

        std::vector<QByteArray> imgParts;
        imgParts.reserve(partsFutures.size());

        for (auto && f: partsFutures) {
          f.waitForFinished();
          imgParts.emplace_back(std::move(f));
        }

      return imgParts;
    });
  }

private:
  static QFuture<QByteArray> PartitionImage(const QImage &img, const QRect &rect)
  {
    return QtConcurrent::run([&]{
              QBuffer buffer;
              buffer.open(QIODevice::WriteOnly);
              img.copy(rect).save(&buffer, "JPEG");
              return buffer.data().toBase64();
    });
  }

};


}
}
#endif // BOARDIMAGEPROCSSOR_H
