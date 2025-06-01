#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Window window;
    engine.rootContext()->setContextProperty("window", &window);

    engine.load(QUrl(QStringLiteral("qrc:/window.qml")));
    if(engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
