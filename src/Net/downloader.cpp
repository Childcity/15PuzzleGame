#include "downloader.h"


namespace Net {


Downloader::Downloader()
{
    deadlineTimer_.setInterval(defaultTimeout);
    networkManager_.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    QObject::connect(&networkManager_, &QNetworkAccessManager::finished, &looper_, &QEventLoop::quit );
    QObject::connect(&deadlineTimer_, &QTimer::timeout, &looper_, &QEventLoop::quit);
}

Downloader::~Downloader()
{}

IDownloader::Result Downloader::get(const QNetworkRequest &request)
{
    DEBUG("reaching url: " << request.url());

    qt_unique_ptr<QNetworkReply> reply;
    reply.reset(networkManager_.get(request));

    deadlineTimer_.setSingleShot(true);
    deadlineTimer_.start();
    looper_.exec();

    if (! deadlineTimer_.isActive()){
        deadlineTimer_.stop();
        DEBUG("NetworkReply timed out!");
        return Result({}, QNetworkReply::TimeoutError);
    }

    return Result(reply->readAll(), reply->error());
}

IDownloader::Result Downloader::get(const QUrl &url, const QString &userAgent)
{
    QNetworkRequest request(url);
    request.setMaximumRedirectsAllowed(5);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    request.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
    return get(request);
}

std::chrono::milliseconds Downloader::getTimeout() const { return milliseconds (deadlineTimer_.interval()); }

void Downloader::setTimeout(const std::chrono::milliseconds timeout) { deadlineTimer_.setInterval(timeout); }


}
