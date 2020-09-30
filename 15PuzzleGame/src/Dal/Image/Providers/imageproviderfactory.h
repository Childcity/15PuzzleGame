#ifndef IMAGEPROVIDERFACTORY_H
#define IMAGEPROVIDERFACTORY_H

#include "imageprovidertype.h"
#include "irundomimageprovider.h"
#include "flickrimageprovider.h"


namespace Dal::Image {


class ImageProviderFactory {
public:
    ImageProviderFactory() = delete;

    IRundomImageProvider *CreateImageProvider(ImageProviderType type, std::shared_ptr<Net::IDownloader> downloader)
    {
        if (type == ImageProviderType::Flickr) {
            return new FlickrImageProvider(downloader);
        } else {
            assert(false);
        }
    }
};


} // namespace Dal::Image

#endif // IMAGEPROVIDERFACTORY_H
