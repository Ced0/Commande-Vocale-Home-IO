#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <qmlhandler.h>
#include "wsainit.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    #ifdef _WIN32
        WSAInit wsa;
    #endif

    QMLHandler handler;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    QObject::connect(&engine, SIGNAL(objectCreated(QObject *, const QUrl &)),
                     &handler, SLOT(setQml(QObject *, const QUrl &)));

    engine.load(url);


    return app.exec();
}
