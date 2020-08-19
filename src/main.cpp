#include "Dal/gameboardmodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVector>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    {
        qmlRegisterType<Dal::GameBoardModel>("GameBoardModel", 1,0, "GameBoardModel");
    }

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/src/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
