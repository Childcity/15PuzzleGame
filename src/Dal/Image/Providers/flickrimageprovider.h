#ifndef FLICKRIMAGEPROVIDER_H
#define FLICKRIMAGEPROVIDER_H

#include "main.h"
#include "irundomimageprovider.h"
#include "Net/networkerror.h"
#include "Net/idownloader.h"

#include <QImage>
#include <memory>
#include <chrono>
#include <regex>


namespace Dal::Image {


class FlickrImageProvider : public IRundomImageProvider {
public:
    FlickrImageProvider(std::shared_ptr<Net::IDownloader> downloader);

    ~FlickrImageProvider() override;

    // IRundomImageProvider interface
public:
    QImage getRundomImage() const override
    {
        using namespace std::chrono_literals;

        downloader_->setTimeout(5s);

        QNetworkRequest request(getImgListUrl_);
        request.setMaximumRedirectsAllowed(5);
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                             QNetworkRequest::NoLessSafeRedirectPolicy);
        request.setRawHeader(QByteArray::fromBase64("QXV0aG9yaXphdGlvbg=="),
                             QByteArray::fromBase64("Q2xpZW50LUlEIDE3OWMzZmJiY2RjOGVjNQ=="));

        QByteArray tmpRes;
        std::list<std::string> urls;

        int attempt = maxDownloadAttempts;
        while (attempt--) {
            try {
                // get json array with links to images
                {
                    tmpRes = downloader_->get(request);
                }

                // parse response with links to different images and fill urls list
                {
                    urls.clear();
                    auto matchesBegin = std::regex_token_iterator(tmpRes.cbegin(), tmpRes.cend(), imgUrlTemplate);
                    auto matchesEnd = std::regex_token_iterator<QByteArray::const_iterator>();
                    std::move(matchesBegin, matchesEnd, std::back_inserter(urls));
                }

                // get random image
                {
                    tmpRes = downloader_->get(
                        QNetworkRequest(
                            QString::fromStdString(
                                std::regex_replace(
                                    getRandomElement(urls), std::regex("\\\\"), ""))));
                }

                // if we here, we have gor image correctly
                break;
            } catch (const Net::NetworkError &ex) {
                DEBUG("Download error:" << ex.what() << "Attempt:" << maxDownloadAttempts - attempt);
                if (attempt < 1)
                    throw;
            }
        }

        return QImage::fromData(tmpRes);
    }

private:
    template<class T>
    const T &getRandomElement(const std::list<T> &urls) const
    {
        size_t rndIndex = getFastNotSecureRandomNum(0, urls.size() - 1);

        auto it = urls.cbegin();
        std::advance(it, rndIndex);

        return *it;
    }

    size_t getFastNotSecureRandomNum(size_t min, size_t max) const
    {
        using namespace std::chrono;
        unsigned now = static_cast<unsigned>(system_clock::now().time_since_epoch().count());
        ::srand(now);
        size_t rnd = static_cast<size_t>(::rand());
        return min + rnd % (max - min + 1);
    }

private:
    static constexpr int maxDownloadAttempts = 5;

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
