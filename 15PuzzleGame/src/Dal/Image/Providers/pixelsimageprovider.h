#pragma once

#include "irundomimageprovider.h"
#include "Net/idownloader.h"

#include <QImage>
#include <regex>


namespace Dal::Image {


class PixelsImageProvider : public IRundomImageProvider {
public:
    explicit PixelsImageProvider(std::shared_ptr<Net::IDownloader> downloader) noexcept;
    ~PixelsImageProvider() override;

    // IRundomImageProvider interface
public:
    QImage getRundomImage() const override;

private:
    static constexpr int maxDownloadAttempts = 3;

    const std::shared_ptr<Net::IDownloader> downloader_;
    const QString getImgListUrl_ { "https://api.pexels.com/v1/search?query=sexy&size=medium&per_page=80" };
    const std::regex imgUrlTemplate {
        R"_(https*:\/\/[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&\/\/=]*)(\.jpg|\.png|\.jpeg))_"
    };
};


} // namespace Dal::Image
