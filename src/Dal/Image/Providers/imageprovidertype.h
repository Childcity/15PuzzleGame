#ifndef IMAGEPROVIDERTYPE_H
#define IMAGEPROVIDERTYPE_H

#include <QObject>
#include <QVariant>

class ImageProviderType {
    Q_GADGET

    ImageProviderType() = delete;

public:
    enum Value
    {
        Flickr,
        Prnhub
    };
    Q_ENUM(Value)

    static ImageProviderType::Value fromVariant(const QVariant &var)
    {
        return var.value<ImageProviderType::Value>();
    }

    static bool isValid(ImageProviderType::Value type)
    {
        return type == ImageProviderType::Flickr ||
               type == ImageProviderType::Prnhub;
    }
};

#endif // IMAGEPROVIDERTYPE_H
