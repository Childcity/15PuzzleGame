#pragma once

#include <QVariant>


namespace Dal::Image {


class ImageProviderTypeClass {
    Q_GADGET

public:
    ImageProviderTypeClass() = delete;

    enum Value
    {
        Invalid,
        Flickr,
        Pixels
    };
    Q_ENUM(Value)

    static Value fromVariant(const QVariant &var)
    {
        return var.value<Value>();
    }

    static bool isValid(Value type)
    {
        return type == Value::Flickr ||
               type == Value::Pixels;
    }
};

using ImageProviderType = ImageProviderTypeClass::Value;


} // namespace Dal::Image
