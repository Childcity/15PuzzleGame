#include "rundomimageproviderbase.h"


namespace Dal::Image {


RundomImageProviderBase::RundomImageProviderBase(std::shared_ptr<Net::IDownloader> downloader)
    : downloader_(std::move(downloader))
{
    if (! downloader_)
        throw std::invalid_argument("downloader mustn't be nullptr");
}

RundomImageProviderBase::~RundomImageProviderBase() {}

std::shared_ptr<Net::IDownloader> RundomImageProviderBase::GetDownloader() const
{
    return downloader_;
}

QString RundomImageProviderBase::GetDefaultUserAgent()
{
    static QString Agent = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:15.0) "
                           "Gecko/20100101 Firefox/15.0.1";
    return Agent;
}

std::regex &RundomImageProviderBase::GetDefaultImageUrlTemplate()
{
    static std::regex ImgUrlTemplate {
        R"_(https*:\\\/\\\/[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&\\\/\\\/=]*)(\.jpg|\.png))_"
    };

    return ImgUrlTemplate;
}


} // namespace Dal::Image
