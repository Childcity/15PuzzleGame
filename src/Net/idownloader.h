#ifndef IDOWNLOADER_H
#define IDOWNLOADER_H

#include <QNetworkReply>


namespace Net {


class IDownloader {
public:
    using Result = std::pair<QByteArray, QNetworkReply::NetworkError>;

public:
    virtual ~IDownloader();
    virtual Result get(const QNetworkRequest &) = 0;
    virtual Result get(const QUrl &, const QString &userAgent = QLatin1Literal("Mozilla/5.0")) = 0;
    virtual std::chrono::milliseconds getTimeout() const = 0;
    virtual void setTimeout(const std::chrono::milliseconds timeout) = 0;
};


}


#endif // IDOWNLOADER_H
