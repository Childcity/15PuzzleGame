#ifndef FLICKRIMAGEPROVIDER_H
#define FLICKRIMAGEPROVIDER_H

#include "main.h"
#include "irundomimageprovider.h"
#include <Net/idownloader.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QImage>
#include <memory>

namespace Dal {
namespace Image {


class FlickrImageProvider : public IRundomImageProvider {
  public:
    FlickrImageProvider(std::shared_ptr<Net::IDownloader> downloader);

    ~FlickrImageProvider() override;

    // IRundomImageProvider interface
  public:
    QImage getRundomImage() const override
    {
        QString userAgent = QLatin1Literal("Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:15.0) "
                                           "Gecko/20100101 Firefox/15.0.1");
        QUrl url("https://api.flickr.com/services/feeds/photos_public.gne"
                 "?tags=nature,sky&tagmode=any&format=json&nojsoncallback=1");

        QNetworkRequest request;
        request.setMaximumRedirectsAllowed(5);
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        request.setHeader(QNetworkRequest::UserAgentHeader, userAgent);

        QByteArray imgSrc;
        QNetworkReply::NetworkError err;

        int maxTryis = 5;
        do {
            request.setUrl(url);
            std::tie(imgSrc, err) = downloader_->get(request);

            Q_UNUSED(err) // TODO: check on error!

            QString imgUrl = QJsonDocument::fromJson(imgSrc)
                                 .object()["items"]
                                 .toArray()
                                 .last()["media"]
                                 .toObject()["m"]
                                 .toString();

            imgUrl.replace("_m", "_b"); // m -> small image, b -> large image

            request.setUrl(imgUrl);
            std::tie(imgSrc, err) = downloader_->get(request);
            Q_UNUSED(err)

        } while (err != QNetworkReply::NoError && --maxTryis);

        return QImage::fromData(imgSrc);
    }

  private:
    std::shared_ptr<Net::IDownloader> downloader_;
};


} // namespace Image
} // namespace Dal

#endif // FLICKRIMAGEPROVIDER_H
