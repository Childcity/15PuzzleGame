#include "PixelsImageProvider.h"
#include "main.h"

#include <QJsonObject>
#include <QJsonDocument>

namespace Dal::Image {


PixelsImageProvider::PixelsImageProvider(std::shared_ptr<Net::IDownloader> downloader)
    : downloader_(std::move(downloader))
{
    if (! downloader_)
        throw std::invalid_argument("downloader mustn't be nullptr");
}

PixelsImageProvider::~PixelsImageProvider()
{
    DEBUG("~PixelsImageProvider");
}
QImage PixelsImageProvider::getRundomImage() const
{
    const int rndPage = getFastNotSecureRandomNum(1, 50);
    const auto rndPageUrl = QString("%1&page=%2").arg(getImgListUrl_).arg(rndPage);

    QNetworkRequest request(rndPageUrl);
    request.setMaximumRedirectsAllowed(5);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::NoLessSafeRedirectPolicy);
    request.setRawHeader(QByteArray::fromBase64("QXV0aG9yaXphdGlvbg=="),
                         QByteArray::fromBase64("NTYzNDkyYWQ2ZjkxNzAwMDAxMDAwMDAxNTdiNGI4NGQ5YzBhNGEwMWJmYjU1NmU0MjE5YzZhZjY="));

    QByteArray tmpRes;
    std::list<std::string> urls;

    int attempt = maxDownloadAttempts;
    while (attempt--) {
        try {
            // get json array with links to images
            {
                tmpRes = downloader_->get(request);
            }

            DEBUG(QJsonDocument::fromJson(tmpRes).toJson(QJsonDocument::JsonFormat::Indented).mid(0, 2048).data());

            // parse response with links to different images and fill urls list
            {
                urls.clear();
                auto matchesBegin = std::regex_token_iterator(tmpRes.cbegin(), tmpRes.cend(), imgUrlTemplate);
                auto matchesEnd = std::regex_token_iterator<QByteArray::const_iterator>();
                std::move(matchesBegin, matchesEnd, std::back_inserter(urls));
            }

            // get random image
            {
                const auto url = QString::fromStdString(getRandomElement(urls)) + "?auto=compress&cs=tinysrgb&h=1200";
                tmpRes = downloader_->get(QNetworkRequest(url));
            }
            DEBUG(QString::fromStdString(urls.front()) << "s"<<urls.size()<<"simage"<<tmpRes.size()<<tmpRes.mid(0, 20))

            // if we here, we have got image correctly
            break;
        } catch (const Net::NetworkError &ex) {
            DEBUG("Download error:" << ex.what() << "Attempt:" << maxDownloadAttempts - attempt);
            if (attempt < 1)
                throw;
        } catch (const std::exception &ex) {
            DEBUG("Download error:" << ex.what() << "Attempt:" << maxDownloadAttempts - attempt);
            if (attempt < 1)
                throw Net::NetworkError(Net::NetworkError::NetErrorCode::UnknownServerError);
        }
    }

    return QImage::fromData(tmpRes);
}

size_t PixelsImageProvider::getFastNotSecureRandomNum(size_t min, size_t max) const
{
    using namespace std::chrono;
    unsigned now = static_cast<unsigned>(system_clock::now().time_since_epoch().count());
    ::srand(now);
    size_t rnd = static_cast<size_t>(::rand());
    return min + rnd % (max - min + 1);
}


} // namespace Dal::Image
