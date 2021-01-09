#pragma once

#include "Dal/Image/Providers/imageprovidertype.h"

#include <QSettings>

class QQmlEngine;
class QJSEngine;

class AppSettings : public QObject {
    Q_OBJECT

    Q_PROPERTY(int dimension READ dimension WRITE setDimension NOTIFY sigDimensionChanged)
    Q_PROPERTY(ImageProviderType imageProvider READ imageProvider WRITE setImageProvider NOTIFY sigImageProviderChanged)

    explicit AppSettings(QObject *parent = nullptr) noexcept;

public:
    using ImageProviderType = Dal::Image::ImageProviderType;

    static AppSettings &Get();
    static QObject *Get(QQmlEngine *, QJSEngine *);

    int dimension() const;
    void setDimension(int dimension);
    ImageProviderType imageProvider();
    void setImageProvider(ImageProviderType imageProvider);

public slots:
    bool isValid() const;
    QString status() const;

signals:
    void sigDimensionChanged(int dimension);
    void sigImageProviderChanged(ImageProviderType imageProvider);

private:
    QSettings settings_;
};
