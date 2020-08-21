#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "main.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <chrono>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPixmap>
#include <QImage>

#include <Net/idownloader.h>

namespace Net {

class Downloader : public IDownloader {
    using milliseconds = std::chrono::milliseconds;
    using seconds = std::chrono::seconds;

public:
    explicit Downloader();

    Downloader(const Downloader &) = delete;

    ~Downloader() override;

    // IDownloader interface
public:
    Result get(const QNetworkRequest &request) override;

    Result get(const QUrl &url) override;

    std::chrono::milliseconds getTimeout() const override;

    void setTimeout(const std::chrono::milliseconds timeout) override;

private:
    static constexpr int defaultTimeout = 3000;

    QNetworkAccessManager networkManager_;
    QEventLoop looper_;
    QTimer deadlineTimer_;
};


class IRundomImageProvider {
public:
    virtual ~IRundomImageProvider(){}
    virtual QImage getRundomImage() const = 0;
};


class FlickrImageProvider : public IRundomImageProvider {
public:
    FlickrImageProvider(std::shared_ptr<IDownloader> downloader)
        : downloader_(std::move(downloader))
    {}

    ~FlickrImageProvider() override
    {}

    // IRundomImageProvider interface
public:
    QImage getRundomImage() const override
    {
        QUrl url("https://api.flickr.com/services/feeds/photos_public.gne"
                 "?tags=nature,sky&tagmode=any&format=json&nojsoncallback=1");

        auto &&[jsonResponseRaw, error1] = downloader_->get(url);{}
        Q_UNUSED(error1)

        QString imgUrl = QJsonDocument::fromJson(jsonResponseRaw).object()
                ["items"].toArray().last()
                ["media"].toObject()
                ["m"].toString();

        imgUrl.replace("_m", "_b"); // m -> small image, b -> large image

        auto &&[imgSrc, error2] = downloader_->get(imgUrl);{}
        Q_UNUSED(error2)

        QImage imgViewer;
        imgViewer.loadFromData(imgSrc);
        return imgViewer;
    }

private:
    std::shared_ptr<IDownloader> downloader_;
};

}


#endif // DOWNLOADER_H
