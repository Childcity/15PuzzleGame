#include "flickrimageprovider.h"
#include "main.h"


namespace Dal::Image {


FlickrImageProvider::FlickrImageProvider(std::shared_ptr<Net::IDownloader> downloader)
    : downloader_(std::move(downloader))
{
    if (! downloader_)
        throw std::invalid_argument("downloader mustn't be nullptr");
}

FlickrImageProvider::~FlickrImageProvider() {}

QImage FlickrImageProvider::getRundomImage() const
{
    QNetworkRequest request(getImgListUrl_);
    request.setMaximumRedirectsAllowed(5);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::NoLessSafeRedirectPolicy);
    request.setRawHeader(QByteArray::fromBase64("QXV0aG9yaXphdGlvbg=="),
                         QByteArray::fromBase64("Q2xpZW50LUlEIDE3OWMzZmJiY2RjOGVjNQ=="));

    QByteArray tmpRes;
    std::list<std::string> urls;

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

    return QImage::fromData(tmpRes);
}

size_t FlickrImageProvider::getFastNotSecureRandomNum(size_t min, size_t max) const
{
    using namespace std::chrono;
    unsigned now = static_cast<unsigned>(system_clock::now().time_since_epoch().count());
    ::srand(now);
    size_t rnd = static_cast<size_t>(::rand());
    return min + rnd % (max - min + 1);
}


} // namespace Dal::Image
