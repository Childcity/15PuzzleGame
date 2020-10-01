#include "downloader.h"

#include "networkerror.h"
#include "operationcancelederror.h"

#include <QNetworkReply>
#include <chrono>
#include <memory>


namespace Net {


Downloader::Downloader(const std::atomic_bool &cancelationRequest)
    : isCancelationRequested_(cancelationRequest)
{
    // Setup deadline timer and networkManager_
    {
        networkManager_ = new QNetworkAccessManager(&looper_);
        deadlineTimer_ = new QTimer(&looper_);

        deadlineTimer_->setSingleShot(true);
        deadlineTimer_->setInterval(defaultTimeout);
        networkManager_->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

        QObject::connect(networkManager_, &QNetworkAccessManager::finished, &looper_, &QEventLoop::quit);
        QObject::connect(deadlineTimer_, &QTimer::timeout, &looper_, &QEventLoop::quit);
    }

    // Setup cancelation checker
    {
        cancelationChacker_ = new QTimer(&looper_);
        cancelationChacker_->setInterval(defaultCancelationCheckerTimeout);
        QObject::connect(cancelationChacker_, &QTimer::timeout, &looper_, [this] {
            if (isCancelationRequested_.load(std::memory_order_relaxed)) {
                cancelationChacker_->stop();
                looper_.quit();
            }
        });
    }
}

Downloader::~Downloader()
{}

QByteArray Downloader::get(const QNetworkRequest &request)
{
    const std::unique_ptr<QNetworkReply> reply(networkManager_->get(request));
    QObject::connect(&*reply, &QNetworkReply::redirected, &*reply, &QNetworkReply::redirectAllowed);
    QObject::connect(deadlineTimer_, &QTimer::timeout, &*reply, &QNetworkReply::abort);

    deadlineTimer_->start();
    cancelationChacker_->start();
    looper_.exec();

    if (isCancelationRequested_.load(std::memory_order_relaxed)) {
        reply->abort();
        deadlineTimer_->stop();
        throw OperationCanceledError();
    }

    if ((! deadlineTimer_->isActive()) || (! reply->isFinished())) {
        deadlineTimer_->stop();
        throw NetworkError(QNetworkReply::TimeoutError);
    }

    if (reply->error() != QNetworkReply::NoError) {
        throw NetworkError(reply->error());
    }

    return reply->readAll();
}

std::chrono::milliseconds Downloader::getTimeout() const
{
    return milliseconds(deadlineTimer_->interval());
}

void Downloader::setTimeout(const std::chrono::milliseconds timeout)
{
    if (timeout.count() <= 0)
        throw std::invalid_argument("Downloader::setTimeout: 'timeout' must be > 0");

    deadlineTimer_->setInterval(timeout);
}


} // namespace Net
