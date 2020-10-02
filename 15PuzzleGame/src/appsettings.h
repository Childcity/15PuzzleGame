#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include "Utils/Utils.h"
#include "Dal/Image/Providers/imageprovidertype.h"

#include <QSettings>
#include <QGuiApplication>
#include <QQmlEngine>
#include <mutex>

using Dal::Image::ImageProviderType;


class AppSettings : public QObject {
    Q_OBJECT

    Q_PROPERTY(int dimension READ dimension WRITE setDimension NOTIFY sigDimensionChanged)
    Q_PROPERTY(ImageProviderType imageProvider READ imageProvider WRITE setImageProvider NOTIFY sigImageProviderChanged)

    explicit AppSettings(QObject *parent = nullptr);

public:
    static AppSettings &Get();

    static QObject *Get(QQmlEngine *, QJSEngine *);

public:
    int dimension() const;

    ImageProviderType imageProvider() const;

signals:
    void sigDimensionChanged(int dimension);

    void sigImageProviderChanged(ImageProviderType imageProvider);

    void sigSettingsError(QString errorString) const;

public slots:
    void setDimension(int dimension);

    void setImageProvider(ImageProviderType imageProvider);

    bool isValid() const;

    QString status() const;

private:
    QSettings settings_;
};

#endif // APPSETTINGS_H
