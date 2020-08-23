#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "main.h"
#include <Net/idownloader.h>

#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QTimer>
#include <chrono>

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

    std::chrono::milliseconds getTimeout() const override;

    void setTimeout(const std::chrono::milliseconds timeout) override;

private:
    static constexpr int defaultTimeout = 3000;

    QNetworkAccessManager networkManager_;
    QEventLoop looper_;
    QTimer deadlineTimer_;
};


}


#endif // DOWNLOADER_H
