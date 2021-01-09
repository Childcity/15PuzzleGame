#pragma once

class QImage;

namespace Dal::Image {


class IRundomImageProvider {
public:
    virtual ~IRundomImageProvider();
    virtual QImage getRundomImage() const = 0;
};


} // namespace Dal::Image
