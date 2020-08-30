#include "downloader.h"
#include "networkerror.h"

#include <QNetworkReply>


namespace Net {


Downloader::Downloader()
{
    deadlineTimer_.setInterval(defaultTimeout);
    networkManager_.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    QObject::connect(&networkManager_, &QNetworkAccessManager::finished, &looper_, &QEventLoop::quit);
    QObject::connect(&deadlineTimer_, &QTimer::timeout, &looper_, &QEventLoop::quit);
}

Downloader::~Downloader()
{}

QByteArray Downloader::get(const QNetworkRequest &request)
{
    DEBUG("Reaching url: " << request.url() << "Timeout:" << getTimeout().count());

    const qt_unique_ptr<QNetworkReply>
        reply(networkManager_.get(request));

    deadlineTimer_.setSingleShot(true);
    deadlineTimer_.start();
    looper_.exec();

    if (! deadlineTimer_.isActive()) {
        deadlineTimer_.stop();
        throw NetworkError(QNetworkReply::TimeoutError);
    }

    if (reply->error() != QNetworkReply::NoError) {
        throw NetworkError(reply->error());
    }

    return reply->readAll();
}

std::chrono::milliseconds Downloader::getTimeout() const
{
    return milliseconds(deadlineTimer_.interval());
}

void Downloader::setTimeout(const std::chrono::milliseconds timeout)
{
    deadlineTimer_.setInterval(timeout);
}


} // namespace Net
