#ifndef RUNDOMIMAGEPROVIDERBASE_H
#define RUNDOMIMAGEPROVIDERBASE_H

#include "irundomimageprovider.h"
#include "Net/networkerror.h"
#include "Net/idownloader.h"

#include <QString>
#include <regex>
#include <list>


namespace Dal::Image {


class RundomImageProviderBase : public IRundomImageProvider {
public:
    explicit RundomImageProviderBase(std::shared_ptr<Net::IDownloader> downloader);

    ~RundomImageProviderBase() override;

    // IRundomImageProvider interface
public:
    QImage getRundomImage() const override = 0;

protected:
    std::shared_ptr<Net::IDownloader> GetDownloader() const;

    static QString GetDefaultUserAgent();

    static std::regex &GetDefaultImageUrlTemplate();

    // parse response with links to different images and fill urls list
    template<typename Iterator>
    static void FindUrlsDefault(const QByteArray &tmpRes, Iterator iter)
    {
        auto matchesBegin = std::regex_token_iterator(tmpRes.cbegin(), tmpRes.cend(), GetDefaultImageUrlTemplate());
        auto matchesEnd = std::regex_token_iterator<QByteArray::const_iterator>();
        std::move(matchesBegin, matchesEnd, iter);
    }

private:
    const std::shared_ptr<Net::IDownloader> downloader_;
};


} // namespace Dal::Image

#endif // RUNDOMIMAGEPROVIDERBASE_H
