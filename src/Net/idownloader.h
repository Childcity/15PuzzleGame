#ifndef IDOWNLOADER_H
#define IDOWNLOADER_H

#include <QNetworkReply>

namespace Net {


class IDownloader {
public:
    virtual ~IDownloader();
    virtual QByteArray get(const QNetworkRequest &) = 0;
    virtual std::chrono::milliseconds getTimeout() const = 0;
    virtual void setTimeout(const std::chrono::milliseconds timeout) = 0;
};


} // namespace Net


#endif // IDOWNLOADER_H
