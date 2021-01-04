#pragma once

#include "irundomimageprovider.h"
#include "Net/networkerror.h"
#include "Net/idownloader.h"

#include <QImage>
#include <memory>
#include <regex>


namespace Dal::Image {


class PHubImageProvider : public IRundomImageProvider {
public:
    explicit PHubImageProvider(std::shared_ptr<Net::IDownloader> downloader);

    ~PHubImageProvider() override;

    // IRundomImageProvider interface
public:
    QImage getRundomImage() const override;

private:
    template<class T>
    const T &getRandomElement(const std::list<T> &urls) const
    {
        if (urls.empty())
            throw std::invalid_argument("urls.size() must be > 0");

        if (urls.size() == 1)
            return *urls.begin();

        size_t rndIndex = getFastNotSecureRandomNum(0, urls.size() - 1);

        auto it = urls.cbegin();
        std::advance(it, rndIndex);

        return *it;
    }

    size_t getFastNotSecureRandomNum(size_t min, size_t max) const;

private:
    static constexpr int maxDownloadAttempts = 5;

    const std::shared_ptr<Net::IDownloader> downloader_;

    // api_key: 563492ad6f9170000100000157b4b84d9c0a4a01bfb556e4219c6af6
//    const QUrl getImgListUrl_ { "https://www.pexels.com/search/erotic/" };

    const QString getImgListUrl_ { "https://api.pexels.com/v1/search?query=sexy&size=medium&per_page=80" };
    const QString userAgent = QLatin1Literal("Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:15.0) "
                                             "Gecko/20100101 Firefox/15.0.1");

    const std::regex imgUrlTemplate {
        R"_(https*:\/\/[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&\/\/=]*)(\.jpg|\.png|\.jpeg))_"
    };
};


} // namespace Dal::Image
