#ifndef PHUBPROVIDER_H
#define PHUBPROVIDER_H

#include "irundomimageprovider.h"
#include "Net/networkerror.h"
#include "Net/idownloader.h"

#include <QImage>
#include <memory>
#include <regex>


namespace Dal::Image {


class PhubProvider : public IRundomImageProvider {
public:
	PhubProvider(std::shared_ptr<Net::IDownloader> downloader)
		: downloader_(std::move(downloader))
	{
		if (! downloader_)
			throw std::invalid_argument("downloader mustn't be nullptr");
	}

	~PhubProvider() override;

	// IRundomImageProvider interface
public:
	QImage getRundomImage() const override
	{
		QNetworkRequest request(QUrl{});
		request.setMaximumRedirectsAllowed(5);
		request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
							 QNetworkRequest::NoLessSafeRedirectPolicy);
		//request.setRawHeader(QByteArray::fromBase64("QXV0aG9yaXphdGlvbg=="),
		//					 QByteArray::fromBase64("Q2xpZW50LUlEIDE3OWMzZmJiY2RjOGVjNQ=="));

		QByteArray tmpRes;
		std::list<std::string> urls;

		// get json array with links to images
		{
			tmpRes = downloader_->get(request);
		}

		// parse response with links to different images and fill urls list
//		{
//			urls.clear();
//			auto matchesBegin = std::regex_token_iterator(tmpRes.cbegin(), tmpRes.cend(), imgUrlTemplate);
//			auto matchesEnd = std::regex_token_iterator<QByteArray::const_iterator>();
//			std::move(matchesBegin, matchesEnd, std::back_inserter(urls));
//		}

//		// get random image
//		{
//			tmpRes = downloader_->get(
//				QNetworkRequest(
//					QString::fromStdString(
//						std::regex_replace(
//							getRandomElement(urls), std::regex("\\\\"), ""))));
//		}

		return QImage::fromData(tmpRes);
	}

private:
	const std::shared_ptr<Net::IDownloader> downloader_;

	const QUrl getImgListUrl_ { "aHR0cHM6Ly93d3cucG9ybmh1Yi5jb20vYWxidW1zL2ZlbWFsZT9zZWFyY2g9aGVudGFp" };

};


} // namespace Dal::Image

#endif // PHUBPROVIDER_H
