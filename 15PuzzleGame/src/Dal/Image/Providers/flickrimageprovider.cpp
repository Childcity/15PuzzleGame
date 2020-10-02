#include "flickrimageprovider.h"

#include "Utils/Utils.h"
#include <QImage>


namespace Dal::Image {


FlickrImageProvider::~FlickrImageProvider() {}

QImage FlickrImageProvider::getRundomImage() const
{
    QNetworkRequest request(getImgListUrl_);
    request.setMaximumRedirectsAllowed(5);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::NoLessSafeRedirectPolicy);
    request.setRawHeader(QByteArray::fromBase64("QXV0aG9yaXphdGlvbg=="),
                         QByteArray::fromBase64("Q2xpZW50LUlEIDE3OWMzZmJiY2RjOGVjNQ=="));

    // get json array with links to images
    QByteArray tmpRes = GetDownloader()->get(request);

    std::list<std::string> urls;
    FindUrlsDefault(tmpRes, std::back_inserter(urls));

    // get random image
    {
        tmpRes = GetDownloader()->get(
            QNetworkRequest(
                QString::fromStdString(
                    std::regex_replace(
                        Utils::GetRandomElement(urls), std::regex("\\\\"), ""))));
    }

    return QImage::fromData(tmpRes);
}


} // namespace Dal::Image
