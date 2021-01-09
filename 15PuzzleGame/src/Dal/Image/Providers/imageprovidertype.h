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
        Imgur,
        Pixels
    };
    Q_ENUM(Value)

    static Value fromVariant(const QVariant &var)
    {
        return var.value<Value>();
    }

    static bool isValid(Value type)
    {
        return type == Value::Imgur ||
               type == Value::Pixels;
    }
};

using ImageProviderType = ImageProviderTypeClass::Value;


} // namespace Dal::Image
