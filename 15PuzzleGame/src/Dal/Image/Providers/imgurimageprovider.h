#pragma once

#include "irundomimageprovider.h"
#include "Net/idownloader.h"

#include <QImage>
#include <regex>


namespace Dal::Image {


class ImgurImageProvider : public IRundomImageProvider {
public:
    explicit ImgurImageProvider(std::shared_ptr<Net::IDownloader> downloader) noexcept;
    ~ImgurImageProvider() override;

    // IRundomImageProvider interface
public:
    QImage getRundomImage() const override;

private:
    static constexpr int maxDownloadAttempts = 5;

    const std::shared_ptr<Net::IDownloader> downloader_;
    const QUrl getImgListUrl_ { "https://api.imgur.com/3/gallery/search/top/0/all/?q_all=nature,wallpaper&q_type=jpeg&q_type=png'" };
    const std::regex imgUrlTemplate {
        R"_(https*:\\\/\\\/[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&\\\/\\\/=]*)(\.jpg|\.png))_"
    };
};


} // namespace Dal::Image
