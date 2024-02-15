#include "mavlink/mavlinkudp.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSslSocket>
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    MavLinkProperties mavlinkProperties;

    MavLinkUDP mavLink(&mavlinkProperties, &app);
    QQmlApplicationEngine engine;


    /*
     *     QObject::connect(mavLink, &MavLinkUDP::connected, [&]() {
        qDebug() << "MavLink connected.";
        // Perform actions after MavLink connection
    });
    QObject::connect(mavLink, &MavLinkUDP::errorOccurred, [&](int errorCode) {
        qDebug() << "MavLink error occurred:" << errorCode;
        // Handle MavLink error
    }); */
    // Initialize MavLink
    QString ipString = "127.0.0.1"; // Example IP address
    int port = 14550; // Example port
    int result = mavLink.initialize(ipString, port);
    if (result < 0) {
        qDebug() << "Initialization failed. Error code:" << result;
        // Handle initialization failure
        //return -1;
    }

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);


    engine.rootContext()->setContextProperty("mavlinkHandler", &mavLink);
    engine.rootContext()->setContextProperty("mavlinkProperties", &mavlinkProperties);

    engine.load(url);

    return app.exec();
}
