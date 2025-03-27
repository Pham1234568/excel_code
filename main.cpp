#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Cuaso.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Cuaso cuaso;
    engine.rootContext()->setContextProperty("cuaso", &cuaso);

    engine.load(QUrl(QStringLiteral("qrc:/cuaso.qml")));
    if(engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
