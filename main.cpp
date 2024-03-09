#include "mavlink/mavlinkudp.h"
#include "qjsonobject.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSslSocket>
#include <iostream>
#include <mavlink/planecontroller.h>
#include <utils/httpclient.h>
#include <utils/teknofestproperties.h>
#include <QJsonDocument>
#include <video/videoitem.h>
#include <QList>
#include <QString>
#include <QDir>


bool isSerialPort(const QString &filename) {
    return filename.startsWith("ttyUSB");
}
std::vector<std::string> get_available_ports() {
    std::vector<std::string> ports;

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
            ports.push_back(str);
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
    MavLinkProperties mavlinkProperties;
    PlaneController planeController;
    HttpClient client;
    TeknofestProperties teknofestProperties;


    mavlinkProperties.setSerialPorts(get_available_ports());

    MavLinkUDP mavLink(&mavlinkProperties, &planeController, &teknofestProperties, &client, &app);


    QQmlApplicationEngine engine;


    teknofestProperties.setSimMode(false);
    //qmlRegisterType<PlaneController>("MyTypes", 1, 0, "MyObject");


    // Example usage
    //client.sendGetRequest("https://jsonplaceholder.typicode.com/posts/1");

    // Example POST request
    //QByteArray postData = R"({"key": "value"})";
    //client.sendPostRequest("https://jsonplaceholder.typicode.com/posts", postData);


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

    QString ipString = "127.0.0.1"; // Example IP address
    int port = 14550; //teknofestProperties.simMode() ? 3131 : 14550;//3131; //3131; //14550; // Example port
    int result = mavLink.initialize(ipString, port);
    if (result < 0) {
        qDebug() << "Initialization failed. Error code:" << result;
        // Handle initialization failure
        //return -1;
    }

    planeController.setQmlContext(engine.rootContext());

    int takimid;
    QByteArray session_id;

    //client.sendAuthRequest("http://replica.neostellar.net/api/giris", "admin2", "mc06yeah2", [&takimid, &session_id](std::pair<int, QByteArray> result) {
    //    takimid = result.first;
    //    session_id = result.second;
    //});


    std::pair<int, QByteArray> pair = client.sendAuthRequest("http://replica.neostellar.net/api/giris", "kullanıcı2", "sifre");
    takimid = pair.first;
    session_id = pair.second;

    //qDebug() << "A: " << takimid;

    teknofestProperties.setsession(session_id);
    teknofestProperties.setTakimid(takimid);

    qDebug() << teknofestProperties.takimid();
    qDebug() << teknofestProperties.session();




    //qDebug() << plane->locationData().takimNumarasi;

    if(mavlinkProperties.connected()){
        qDebug() << "Connected to mavlink.";
        if(teknofestProperties.simMode()){
            //plane* plane = mavLink.createDummyPlane();
            plane* plane = planeController.planes()[0];

            QJsonObject json = plane->toJson();

            //qDebug() << "------";
            //qDebug() << json;
            //qDebug() << "------";
            QString serverUrl = "http://replica.neostellar.net/api/telemetri_gonder"; // Example server URL
            client.sendLocationData(serverUrl, &teknofestProperties, &planeController, json);
        }
        //videoItem.startTimer(1000);
    }



    engine.load(url);




    return app.exec();
}
