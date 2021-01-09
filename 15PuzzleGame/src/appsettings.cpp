#include "appsettings.h"
#include "main.h"

#include <QGuiApplication>
#include <QMetaEnum>
#include <mutex>

using ImageProviderTypeClass = Dal::Image::ImageProviderTypeClass;

namespace Cnst {

using LL = QLatin1Literal;
const auto dimension = LL("dimension");
const auto imageProvider = LL("imageProvider");

}

AppSettings::AppSettings(QObject *parent) noexcept
    : QObject(parent)
    , settings_(QCoreApplication::applicationDirPath() +
                    "/" + QGuiApplication::applicationName() +
                    ".ini", QSettings::Format::IniFormat)
{}

AppSettings &AppSettings::Get()
{
    static AppSettings *pthis;
    static std::once_flag flag;

    std::call_once(flag, []{
        // QQmlEngine will automaticaly delete AppSettings!
        pthis = new AppSettings;
    });

    return *pthis;
}

QObject *AppSettings::Get(QQmlEngine *, QJSEngine *)
{
    return &AppSettings::Get();
}

int AppSettings::dimension() const
{
    return settings_.value(Cnst::dimension, 4).toInt();
}

void AppSettings::setDimension(int dimension)
{
    if (this->dimension() == dimension)
        return;

    settings_.setValue(Cnst::dimension, dimension);
    emit sigDimensionChanged(dimension);
}

AppSettings::ImageProviderType AppSettings::imageProvider()
{
    static const auto defaultType = ImageProviderType::Flickr;

    auto type = ImageProviderTypeClass::fromVariant(
        settings_.value(Cnst::imageProvider, defaultType));

    if (! ImageProviderTypeClass::isValid(type)) {
        settings_.setValue(Cnst::imageProvider, defaultType);
        type = defaultType;
    }

    return type;
}

void AppSettings::setImageProvider(ImageProviderType imageProvider)
{
    if (this->imageProvider() == imageProvider)
        return;

    settings_.setValue(Cnst::imageProvider, imageProvider);
    emit sigImageProviderChanged(imageProvider);
}

bool AppSettings::isValid() const
{
    if (! settings_.isWritable() || settings_.status() != QSettings::Status::NoError) {
        DEBUG(settings_.status() << settings_.isWritable());
        return false;
    }
    return true;
}

QString AppSettings::status() const
{
    return QMetaEnum::fromType<QSettings::Status>().valueToKey(settings_.status());
}
