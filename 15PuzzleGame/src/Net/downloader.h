#pragma once

#include "Net/idownloader.h"

#include <QEventLoop>

class QNetworkAccessManager;
class QTimer;


namespace Net {


class Downloader : public IDownloader {
    using milliseconds = std::chrono::milliseconds;

public:
    explicit Downloader(const std::atomic_bool &cancelationRequest);
    Downloader(const Downloader &) = delete;
    ~Downloader() override = default;

    // IDownloader interface
public:
    QByteArray get(const QNetworkRequest &request) override;
    std::chrono::milliseconds getTimeout() const override;
    void setTimeout(const std::chrono::milliseconds timeout) override;

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
