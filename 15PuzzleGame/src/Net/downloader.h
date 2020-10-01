#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "Net/idownloader.h"

#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QTimer>


namespace Net {


class Downloader : public IDownloader {
    using milliseconds = std::chrono::milliseconds;

public:
    explicit Downloader(const std::atomic_bool &isCanceled);

    Downloader(const Downloader &) = delete;

    ~Downloader() override;

    // IDownloader interface
public:
    QByteArray get(const QNetworkRequest &request) override;

    std::chrono::milliseconds getTimeout() const override;

    void setTimeout(const std::chrono::milliseconds timeout) override;

    //void setCancelation()

private:
    static constexpr int defaultTimeout = 30000;
    static constexpr int defaultCancelationCheckerTimeout = 100;

    QEventLoop looper_;
    QNetworkAccessManager *networkManager_;
    QTimer *deadlineTimer_;
    QTimer *cancelationChacker_;

    const std::atomic_bool &isCancelationRequested_;
};


} // namespace Net


#endif // DOWNLOADER_H
