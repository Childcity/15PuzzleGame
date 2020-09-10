#include "appsettings.h"

AppSettings::AppSettings(QObject *parent)
    : QObject(parent)
    , settings_("./" + QGuiApplication::applicationName() + ".ini", QSettings::Format::IniFormat)
{}
