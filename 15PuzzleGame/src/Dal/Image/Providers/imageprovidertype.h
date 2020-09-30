#ifndef IMAGEPROVIDERTYPE_H
#define IMAGEPROVIDERTYPE_H

#include <QObject>
#include <QVariant>


namespace Dal::Image {


class ImageProviderTypeClass {
    Q_GADGET

public:
    ImageProviderTypeClass() = delete;

    enum Value
    {
        Flickr,
        Phub
    };
    Q_ENUM(Value)

    static Value fromVariant(const QVariant &var)
    {
        return var.value<ImageProviderTypeClass::Value>();
    }

    static bool isValid(ImageProviderTypeClass::Value type)
    {
        return type == ImageProviderTypeClass::Value::Flickr ||
               type == ImageProviderTypeClass::Value::Phub;
    }
};

using ImageProviderType = ImageProviderTypeClass::Value;


} // namespace Dal::Image

#endif // IMAGEPROVIDERTYPE_H
