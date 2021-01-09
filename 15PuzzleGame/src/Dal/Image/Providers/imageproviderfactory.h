#pragma once

#include "imageprovidertype.h"
#include "Net/idownloader.h"
#include "irundomimageprovider.h"

#include <memory>


namespace Dal::Image {


struct ImageProviderFactory {
    ImageProviderFactory() = delete;
    static std::unique_ptr<IRundomImageProvider> GetImageProvider(ImageProviderType type,
                                                                  std::shared_ptr<Net::IDownloader> downloader);
};


} // namespace Dal::Image
