#ifndef FLICKRIMAGEPROVIDER_H
#define FLICKRIMAGEPROVIDER_H

#include "irundomimageprovider.h"
#include "Net/networkerror.h"
#include "Net/idownloader.h"

#include <QImage>
#include <memory>
#include <regex>


namespace Dal::Image {


class FlickrImageProvider : public IRundomImageProvider {
public:
    explicit FlickrImageProvider(std::shared_ptr<Net::IDownloader> downloader);

    ~FlickrImageProvider() override;

    // IRundomImageProvider interface
public:
    QImage getRundomImage() const override;

private:
    template<class T>
    const T &getRandomElement(const std::list<T> &urls) const
    {
        size_t rndIndex = getFastNotSecureRandomNum(0, urls.size() - 1);

        auto it = urls.cbegin();
        std::advance(it, rndIndex);

        return *it;
    }

    size_t getFastNotSecureRandomNum(size_t min, size_t max) const;

private:
    static constexpr int maxDownloadAttempts = 3;

    const std::shared_ptr<Net::IDownloader> downloader_;

    const QUrl getImgListUrl_ { "https://api.imgur.com/3/gallery/search/top/0/all/?q_all=nature,wallpaper&q_type=jpeg&q_type=png'" };
    const QString userAgent = QLatin1Literal("Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:15.0) "
                                             "Gecko/20100101 Firefox/15.0.1");

    const std::regex imgUrlTemplate {
        R"_(https*:\\\/\\\/[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&\\\/\\\/=]*)(\.jpg|\.png))_"
    };
};


} // namespace Dal::Image

#endif // FLICKRIMAGEPROVIDER_H
