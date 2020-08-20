#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "main.h"

#include <memory>
#include <chrono>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtConcurrent>
#include <QEventLoop>
#include <QTimer>


namespace Net {


class IDownloader {
public:
    using Result = std::pair<QByteArray, QNetworkReply::NetworkError>;

public:
    virtual ~IDownloader();
    virtual QFuture<Result> get(const QNetworkRequest &) = 0;
    virtual QFuture<Result> get(const QUrl &) = 0;
    virtual std::chrono::milliseconds getTimeout() const = 0;
    virtual void setTimeout(const std::chrono::milliseconds timeout) = 0;
};

class Downloader : public IDownloader {
private:
    using milliseconds = std::chrono::milliseconds;
    using seconds = std::chrono::seconds;

public:
    explicit Downloader()
    {
        timeout_ = milliseconds(3000);
    }

    Downloader(const Downloader &) = delete;

    ~Downloader() override;

    // IDownloader interface
public:
    QFuture<Result> get(const QNetworkRequest &request) override
    {
        return QtConcurrent::run([this](const QNetworkRequest request){
            QNetworkAccessManager networkManager;
            QEventLoop looper;
            QTimer deadlineTimer;

            networkManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
            deadlineTimer.setSingleShot(true);

            QObject::connect(&networkManager, &QNetworkAccessManager::finished, &looper, &QEventLoop::quit );
            QObject::connect(&deadlineTimer, &QTimer::timeout, &looper, &QEventLoop::quit);

            //DEBUG("reaching url: " << request.url());
            qt_unique_ptr<QNetworkReply> reply;
            reply.reset(networkManager.get(request));

            deadlineTimer.start(timeout_);
            looper.exec();

            if (! deadlineTimer.isActive()){
                deadlineTimer.stop();
                //DEBUG("NetworkReply timed out!");
                return Result({}, QNetworkReply::TimeoutError);
            }

            //DEBUG("get -> " << url << ", size: " << reply->bytesAvailable()
            //      <<"status: " <<reply->error()
            //      <<"deadlineTimer: "<<deadlineTimer.interval()-deadlineTimer.remainingTime());

            return Result(reply->readAll(), reply->error());
       }, request);
    }

    QFuture<Result> get(const QUrl &url) override
    {
        QNetworkRequest request(url);
        request.setMaximumRedirectsAllowed(5);
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        return get(request);
    }

    std::chrono::milliseconds getTimeout() const override { return timeout_; }

    void setTimeout(const std::chrono::milliseconds timeout) override
    {
        timeout_ = timeout;
    }

private:
    std::atomic<milliseconds> timeout_;
};


}


#endif // DOWNLOADER_H
