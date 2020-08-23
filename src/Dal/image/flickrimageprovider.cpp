#include "flickrimageprovider.h"


namespace Dal {
namespace Image {


FlickrImageProvider::FlickrImageProvider(std::shared_ptr<Net::IDownloader> downloader)
  : downloader_(std::move(downloader))
{
  if (! downloader_)
    throw std::invalid_argument("downloader mustn't be nullptr");
}

FlickrImageProvider::~FlickrImageProvider()
{}


}
}
