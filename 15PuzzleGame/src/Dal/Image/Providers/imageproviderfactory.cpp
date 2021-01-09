#include "imageproviderfactory.h"
#include "imgurimageprovider.h"
#include "pixelsimageprovider.h"


namespace Dal::Image {


std::unique_ptr<IRundomImageProvider> ImageProviderFactory::GetImageProvider(ImageProviderType type,
                                                                             std::shared_ptr<Net::IDownloader> downloader)
{
    if (type == ImageProviderType::Imgur) {
        return std::make_unique<ImgurImageProvider>(std::move(downloader));

    } else if (type == ImageProviderType::Pixels) {
        return std::make_unique<PixelsImageProvider>(std::move(downloader));
    }

    assert(false);
    return nullptr;
}


} // namespace Dal::Image
