#include "flickrimageprovider.h"
#include "Net/networkerror.h"

#include "../utils.hpp"
#include "main.h"


namespace Dal::Image {


FlickrImageProvider::FlickrImageProvider(std::shared_ptr<Net::IDownloader> downloader) noexcept
    : downloader_(std::move(downloader))
{
    Q_ASSERT_X(downloader_, __func__, "downloader mustn't be nullptr");
}

FlickrImageProvider::~FlickrImageProvider()
{
    DEBUG("~FlickrImageProvider");
}

QImage FlickrImageProvider::getRundomImage() const
{
    QNetworkRequest request(getImgListUrl_);
    request.setMaximumRedirectsAllowed(5);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::NoLessSafeRedirectPolicy);
    request.setRawHeader(QByteArray::fromBase64("QXV0aG9yaXphdGlvbg=="),
                         QByteArray::fromBase64("Q2xpZW50LUlEIDE3OWMzZmJiY2RjOGVjNQ=="));

    QByteArray tmpRes;

    int attempt = maxDownloadAttempts;
    while (attempt--) {
        try {

            tmpRes = downloader_->get(request);

            const auto urls = findAllMatches(tmpRes, imgUrlTemplate);

            // get random image
            const auto url = std::regex_replace(getRandomElement(urls), std::regex("\\\\"), "");
            tmpRes = downloader_->get(QNetworkRequest(QString::fromStdString(url)));

            // if we are here, we have got image correctly
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
