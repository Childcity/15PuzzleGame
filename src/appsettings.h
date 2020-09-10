#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include "main.h"
#include <Dal/Image/Providers/imageprovidertype.h>

#include <QSettings>
#include <QGuiApplication>
#include <QQmlEngine>
#include <mutex>


class AppSettings : public QObject {
    Q_OBJECT

    Q_PROPERTY(int dimension READ dimension WRITE setDimension NOTIFY sigDimensionChanged)
    Q_PROPERTY(ImageProviderType::Value imageProvider READ imageProvider WRITE setImageProvider NOTIFY sigImageProvider)

    explicit AppSettings(QObject *parent = nullptr);

public:
    static AppSettings &Get()
    {
        static AppSettings *pthis;
        static std::mutex m;

        std::lock_guard lock(m);

        if (pthis == nullptr) // avoid creation of new instances
            pthis = new AppSettings;

        return *pthis;
    }

    static QObject *Get(QQmlEngine *, QJSEngine *)
    {
        return &AppSettings::Get();
    }

public:
    int dimension() const
    {
        return settings_.value("dimension", 4).toInt();
    }

    ImageProviderType::Value imageProvider() const
    {
        auto type = ImageProviderType::fromVariant(
            settings_.value("imageProvider", ImageProviderType::Flickr));

        // check if type is valid
        type = ImageProviderType::isValid(type)
                   ? type
                   : ImageProviderType::Flickr;

        return type;
    }

signals:
    void sigDimensionChanged(int dimension);

    void sigImageProvider(ImageProviderType::Value imageProvider);

    void sigSettingsError(QString errorString) const;

public slots:
    void setDimension(int dimension)
    {
        if (this->dimension() == dimension)
            return;

        //emit sigSettingsError(QString() + settings_.status());

        settings_.setValue("dimension", dimension);
        emit sigDimensionChanged(dimension);
    }

    void setImageProvider(ImageProviderType::Value imageProvider)
    {
        if (this->imageProvider() == imageProvider)
            return;

        settings_.setValue("imageProvider", imageProvider);
        emit sigImageProvider(imageProvider);
    }

    bool isValid() const
    {
        if (! settings_.isWritable() || settings_.status() != QSettings::Status::NoError) {
            DEBUG(settings_.status() << settings_.isWritable())
        }
        return true;
    }

    QString status() const
    {
        return QMetaEnum::fromType<QSettings::Status>().valueToKey(settings_.status());
    }

private:
    QSettings settings_;
};

#endif // APPSETTINGS_H
