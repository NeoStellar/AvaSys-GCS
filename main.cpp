#include "mavlink/mavlinkudp.h"
#include "qjsonobject.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSslSocket>
#include <mavlink/planecontroller.h>
#include <utils/httpclient.h>
#include <utils/notificationcenter.h>
#include <utils/teknofestproperties.h>
#include <QJsonDocument>
#include <video/videoitem.h>
#include <QList>
#include <QString>
#include <QDir>
#include <QMetaType>
#include <horizon/linearindicator.h>



bool isSerialPort(const QString &filename) {
    return filename.startsWith("ttyUSB") || filename.startsWith("ttyACM");
}
QStringList get_available_ports() {
    //std::vector<std::string> ports;
    QStringList ports;

    // Directory where serial port files are located
    const QString devDir = "/sys/class/tty";

    QDir dir(devDir);

    // Get the list of files in the directory
    QStringList fileList = dir.entryList();

    // Iterate through all files in the directory
    for (const QString &entry : fileList) {
        QString filePath = dir.absoluteFilePath(entry);
        QFileInfo fileInfo(filePath);
        if (isSerialPort(entry)) {
            qDebug() << fileInfo;
            std::string str = filePath.toStdString();
            str.erase(0, 15);
            ports << QString::fromStdString(str);
        }
    }
    return ports;
}


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    VideoItem videoItem;
    qmlRegisterType<VideoItem>("CustomTypes", 1, 0, "VideoItem");
    qmlRegisterType<LinearIndicator>("io.smth", 1, 0, "LinearIndicator");
    qmlRegisterType<ColorSegment>("io.smth", 1, 0, "ColorSegment");
    NotificationCenter notificationCenter;
    MavLinkProperties mavlinkProperties;
    PlaneController planeController(&notificationCenter);
    HttpClient client;
    TeknofestProperties teknofestProperties;
    teknofestProperties.setQrLongitude(-122.25);
    mavlinkProperties.setPorts(get_available_ports());
    MavLinkUDP mavLink(&mavlinkProperties, &planeController, &teknofestProperties, &client, &notificationCenter, &app);
    QQmlApplicationEngine engine;



    teknofestProperties.setSimMode(false);

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
    engine.rootContext()->setContextProperty("teknofestProperties", &teknofestProperties);
    engine.rootContext()->setContextProperty("stringList", QVariant::fromValue(mavlinkProperties.ports()));
    engine.rootContext()->setContextProperty("NotificationCenter", &notificationCenter);
    QString ipString = "127.0.0.1"; // Example IP address

    int port = 14550;

    int result = mavLink.initialize(ipString, port);
    if (result < 0) {
        // Try mavproxy
        result = mavLink.initialize(ipString, 3131);
        if(result < 0){
            qDebug() << "Initialization failed. Error code:" << result;
        }
        // Handle initialization failure
        //return -1;
    }else {
        //mavLink.initTeknofest();
    }


    planeController.setQmlContext(engine.rootContext());

    if(mavlinkProperties.connected()){
        qDebug() << "Connected to mavlink.";
        /*
         * This can not be used because they need to specify.
         * if(teknofestProperties.simMode()){
            //plane* plane = mavLink.createDummyPlane();
            plane* plane = planeController.findMainPlane(teknofestProperties.takimid());

            QJsonObject json = plane->toJson();

            //qDebug() << "------";
            //qDebug() << json;
            //qDebug() << "------";
            QString serverUrl = "http://replica.neostellar.net/api/telemetri_gonder"; // Example server URL

            client.sendLocationData(&teknofestProperties, &planeController, json);
        } */
    }
    engine.load(url);
    return app.exec();
}
