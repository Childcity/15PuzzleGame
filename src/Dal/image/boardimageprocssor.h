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


class BoardImageProcssor {
  using IImageProviderPtr = std::unique_ptr<IRundomImageProvider>;

public:
  BoardImageProcssor()
  {
  }

  QFuture<std::vector<QByteArray>> getPartitions(const QPoint &dimensions)
  {
    return QtConcurrent::run([dimensions]{
      auto downloader = std::make_shared<Net::Downloader>();
      IImageProviderPtr imgProvider_ = std::make_unique<FlickrImageProvider>(downloader);


      const QImage imgViewer(imgProvider_->getRundomImage());

      int w = imgViewer.width();
      int h = imgViewer.height();
      int partW = w / dimensions.x();
      int partH = h / dimensions.y();

      int wBarier = w - partW/2;
      int hBarier = h - partH/2;

      std::vector<QFuture<QByteArray>> partsFutures;
      partsFutures.reserve(dimensions.x() * dimensions.y());

        for (int yi = 0; yi < hBarier; yi += partH) {
          for (int xi = 0; xi < wBarier; xi += partW) {
            partsFutures.emplace_back(QtConcurrent::run([&, xi, yi]{
              QImage part(imgViewer.copy(xi, yi, partW, partH));
              QBuffer buffer;
              buffer.open(QIODevice::WriteOnly);
              part.save(&buffer, "JPEG");
              return buffer.data().toBase64();
              //imgParts.emplace_back(bArray.toBase64());
            }));
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
};


}
}

#endif // BOARDIMAGEPROCSSOR_H
