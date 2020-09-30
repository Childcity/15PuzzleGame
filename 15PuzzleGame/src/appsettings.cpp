#include "appsettings.h"

//using namespace Dal::Image;

AppSettings::AppSettings(QObject *parent)
    : QObject(parent)
    , settings_("./" + QGuiApplication::applicationName() + ".ini", QSettings::Format::IniFormat)
{}

AppSettings &AppSettings::Get()
{
    static AppSettings *pthis;
    static std::mutex m;

    std::lock_guard lock(m);

    if (pthis == nullptr) // avoid creation of new instances
        pthis = new AppSettings;

    return *pthis;
}

QObject *AppSettings::Get(QQmlEngine *, QJSEngine *)
{
    return &AppSettings::Get();
}

int AppSettings::dimension() const
{
    return settings_.value("dimension", 4).toInt();
}

ImageProviderType AppSettings::imageProvider() const
{
    //auto type = ImageProviderTypeClass::fromVariant(
    //            settings_.value("imageProvider", ImageProviderType::Flickr));
    //
    //// check if type is valid
    //type = ImageProviderTypeClass::isValid(type)
    //        ? type
    //        : ImageProviderType::Flickr;
    //
    //return type;
}

void AppSettings::setDimension(int dimension)
{
    if (this->dimension() == dimension)
        return;

    //emit sigSettingsError(QString() + settings_.status());

    settings_.setValue("dimension", dimension);
    emit sigDimensionChanged(dimension);
}

void AppSettings::setImageProvider(ImageProviderTypeClass::Value imageProvider)
{
    if (this->imageProvider() == imageProvider)
        return;

    settings_.setValue("imageProvider", imageProvider);
    emit sigImageProviderChanged(imageProvider);
}

bool AppSettings::isValid() const
{
    if (! settings_.isWritable() || settings_.status() != QSettings::Status::NoError) {
        DEBUG(settings_.status() << settings_.isWritable())
    }
    return true;
}

QString AppSettings::status() const
{
    return QMetaEnum::fromType<QSettings::Status>().valueToKey(settings_.status());
}
