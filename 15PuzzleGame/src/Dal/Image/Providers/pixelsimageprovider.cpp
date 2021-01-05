#include "PixelsImageProvider.h"
#include "../utils.hpp"
#include "Net/networkerror.h"
#include "main.h"


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

            tmpRes = downloader_->get(request);

            const auto urls = findAllMatches(tmpRes, imgUrlTemplate);

            // get random image
            {
                const auto url = QString::fromStdString(getRandomElement(urls)) + "?auto=compress&cs=tinysrgb&h=1200";
                tmpRes = downloader_->get(QNetworkRequest(url));
            }

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


} // namespace Dal::Image
