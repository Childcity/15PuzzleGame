#ifndef FLICKRIMAGEPROVIDER_H
#define FLICKRIMAGEPROVIDER_H

#include "rundomimageproviderbase.h"


namespace Dal::Image {


class FlickrImageProvider : public RundomImageProviderBase {
public:
    using RundomImageProviderBase::RundomImageProviderBase;

    ~FlickrImageProvider() override;

    // IRundomImageProvider interface
public:
    QImage getRundomImage() const override;

private:
    const QUrl getImgListUrl_ { "https://api.imgur.com/3/gallery/search/top/0/all/?q_all=nature,wallpaper&q_type=jpeg&q_type=png'" };
};


} // namespace Dal::Image

#endif // FLICKRIMAGEPROVIDER_H
