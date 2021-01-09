#include "Dal/Image/Providers/imageprovidertype.h"
#include "Dal/gameboardmodel.h"
#include "gamecontroller.h"
#include "appsettings.h"

#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    {
        // set default font
        app.setFont({ "Ubuntu", 12 });
    }

    {
        qmlRegisterSingletonType<AppSettings>("AppSettings", 1, 0, "AppSettings", AppSettings::Get);
        qmlRegisterType<Dal::GameBoardModel>("GameBoardModel", 1, 0, "GameBoardModel");
        qmlRegisterType<GameController>("GameController", 1, 0, "GameController");
        qmlRegisterUncreatableType<Dal::Image::ImageProviderTypeClass>("ImageProviderType", 1, 0, "ImageProviderType", "ImageProviderType is a Enum and can't b instantiated!");
        qRegisterMetaType<Dal::Image::ImageProviderType>("ImageProviderType");
    }

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/src/Qml/MainWindow.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
