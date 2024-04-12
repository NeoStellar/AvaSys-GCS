#include "mavlinkudp.h"
#include "mavlink-out/common/mavlink.h"
#include "qdebug.h"
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <unistd.h>
#include <utils/httpclient.h>

MavLinkUDP::MavLinkUDP(MavLinkProperties *mavlinkProperties, PlaneController *planeController, TeknofestProperties *teknofestProperties, HttpClient *httpClient, NotificationCenter* notificationCenter, QObject *parent)
    : QObject(parent),
    m_udpManager(mavlinkProperties),
    m_serialManager(mavlinkProperties),
    m_mavLinkProperties(mavlinkProperties),
    m_planeController(planeController),
    m_teknofestProperties(teknofestProperties),
    m_httpClient(httpClient),
    m_notificationCenter(notificationCenter)
{
    qRegisterMetaType<int32_t>("int32_t");
    qRegisterMetaType<std::vector<plane*>>("std::vector<TeknoCircles*>");
    connect(m_mavLinkProperties, &MavLinkProperties::armedStatusChanged, this, &MavLinkUDP::armedChanged);


    // these are examples
    TeknoCircles* tekno = new TeknoCircles( 37.53, -122.31, 1000, "Yasaklı Alan", 2);
    tekno->setEmpty(false);
    tekno->setBorderColor("black");
    tekno->setBorderSize(300);
    tekno->setInsideColor("darkred");
    tekno->setOpacity(0.8);
    m_circles.push_back(tekno);
    TeknoCircles* tekno2 = new TeknoCircles( 37.53, -122.31, 1200, "", 1);
    tekno2->setEmpty(false);
    tekno2->setInsideColor("lightblue");
    tekno2->setBorderColor("black");
    tekno2->setBorderSize(5000);
    tekno2->setOpacity(0.5);
    m_circles.push_back(tekno2);
    emit circlesChanged();
}

MavLinkUDP::~MavLinkUDP()
{

}
void MavLinkUDP::connectionErrorHandling(int result){
    emit errorOccurred(result);
    qDebug() << result;
    QString text;
    text.append("Error occured with initialization!").append("\n");
    text.append("Code: ").append(QString::number(result));
    switch (result){
    case -3:
        text.append(" (Socket receive failed.)").append("\n");
        text.append("Are you sure vehicle has connection\nwith this address?");
        break;
    case -2:
        text.append(" (Socket bind failed.)").append("\n");
        text.append("Are you sure vehicle does NOT have any\nconnection binded with this address?");
        break;
    default:
        break;
    }

    m_notificationCenter->sendNotification(text, m_notificationCenter->Fatal);
}
int MavLinkUDP::connectSerial(const QString &serialPort, int baudRate)
{

    int result = m_serialManager.initialize(serialPort, baudRate);
    if(result < 0 ){
        connectionErrorHandling(result);
        return result;
    }
    connect(&m_serialManager, &SerialManager::locationDataRecieved, this, &MavLinkUDP::locationDataRecieved);
    connect(&m_serialManager, &SerialManager::yawDataRecieved, this, &MavLinkUDP::yawDataRecieved);
    connect(&m_serialManager, &SerialManager::airspeedDataReceived, this, &MavLinkUDP::airspeedDataReceived);
    connect(&m_serialManager, &SerialManager::pressureDataReceived, this, &MavLinkUDP::pressureDataReceived);
    connect(&m_serialManager, &SerialManager::imageCapturedSignal, this, &MavLinkUDP::imageCapturedSignal);
    connect(&m_serialManager, &SerialManager::batteryDataReceived, this, &MavLinkUDP::batteryDataReceived);

    m_mavLinkProperties->setConnected(true);
    m_mavLinkProperties->setIsSerial(true);
    m_mavLinkProperties->setSysid(m_teknofestProperties->takimid());

    connect(&m_heartbeatTimer, &QTimer::timeout, this
            , &MavLinkUDP::sendHeartbeat);
    m_heartbeatTimer.start(1000);

    // we need to get heartbeat to get system id first.


    //initTeknofest();
    return 1;
}

void MavLinkUDP::initTeknofest(QString url, QString username, QString password){
    if(m_teknofestProperties->simMode()){
        return;
    }
    m_teknofestProperties->setSimMode(true);
    m_httpClient->setUrl(url);
    m_httpClient->setUsername(username);
    m_httpClient->setPassword(password);
    //std::pair<int, QByteArray> pair = m_httpClient->sendAuthRequest("http://replica.neostellar.net/api/giris", "kullanıcı2", "sifre");
    std::pair<int, QByteArray> pair = m_httpClient->sendAuthRequest();
    int takimid = pair.first;
    QByteArray session_id = pair.second;

    m_teknofestProperties->setsession(session_id);
    m_teknofestProperties->setTakimid(takimid);

    qDebug() << m_teknofestProperties->takimid();
    qDebug() << m_teknofestProperties->session();

    if (session_id.isNull() || session_id.isEmpty()){
        qDebug() << "Authentication failed!";
        m_notificationCenter->sendNotification("Authentication Failed!", m_notificationCenter->Error);
        m_teknofestProperties->setSimMode(false);
        return;
    }

    plane* plane = m_planeController->findMainPlane(m_teknofestProperties->takimid(), true);

    qDebug() << "plane:" << plane->airspeed();
    QJsonObject json = plane->toJson();

    //QString serverUrl = url + "/telemetri_gonder"; // Example server URL
    int result = m_httpClient->sendLocationData(m_teknofestProperties, m_planeController, json);
    if (result != 0){
        m_teknofestProperties->setSimMode(false);
        m_notificationCenter->sendNotification("Trial to send location data has been failed!", m_notificationCenter->Error);
    }
}

int MavLinkUDP::initialize(const QString& ipString, int port) {
    int result = m_udpManager.initialize(ipString, port);
    if (result < 0) {
        connectionErrorHandling(result);
        return result;
    }

    connect(&m_udpManager, &UDPManager::locationDataRecieved, this, &MavLinkUDP::locationDataRecieved);
    connect(&m_udpManager, &UDPManager::yawDataRecieved, this, &MavLinkUDP::yawDataRecieved);
    connect(&m_udpManager, &UDPManager::airspeedDataReceived, this, &MavLinkUDP::airspeedDataReceived);
    connect(&m_udpManager, &UDPManager::pressureDataReceived, this, &MavLinkUDP::pressureDataReceived);
    connect(&m_udpManager, &UDPManager::imageCapturedSignal, this, &MavLinkUDP::imageCapturedSignal);
    connect(&m_udpManager, &UDPManager::batteryDataReceived, this, &MavLinkUDP::batteryDataReceived);
    connect(&m_udpManager, &UDPManager::flyingStateChanged, this, &MavLinkUDP::flyingStateChanged);

    // Connect signals from MavLinkProperties to MavLink
    connect(m_mavLinkProperties, &MavLinkProperties::connectedChanged, this, &MavLinkUDP::connectedChanged);

    m_mavLinkProperties->setConnected(true);
    m_mavLinkProperties->setSysid(m_teknofestProperties->takimid());

    connect(&m_heartbeatTimer, &QTimer::timeout, this
            , &MavLinkUDP::sendHeartbeat);
    m_heartbeatTimer.start(1000);

    //initTeknofest();
    return 1;
}

void MavLinkUDP::locationDataRecieved(int sysid, float latitude, float longitude, int32_t altitude){
    m_planeController->setTeam(sysid, !m_teknofestProperties->simMode() ? -1 : m_teknofestProperties->takimid());
    m_planeController->addOrUpdatePlane(sysid, latitude, longitude, altitude, m_teknofestProperties->simMode());
    m_teknofestProperties->addPlane(sysid);
}

void MavLinkUDP::yawDataRecieved(int sysid, float yaw){
    m_planeController->updateYaw(sysid, yaw);
}
void MavLinkUDP::airspeedDataReceived(int id, float speed){
    m_planeController->updateSpeed(id, speed);
}
void MavLinkUDP::pressureDataReceived(int id, float pressure){
    m_planeController->updatePressure(id, pressure);
}
void MavLinkUDP::batteryDataReceived(int sysid, float voltage, float current, float remainingCapacity){
    m_planeController->updateBattery(sysid, voltage, current, remainingCapacity);
}

void MavLinkUDP::onHeartbeatTimeout() {
    sendHeartbeat();
}

void MavLinkUDP::sendHeartbeat() {

    if(m_mavLinkProperties->connected()){
        send_heartbeat();
        if(m_teknofestProperties->simMode()){
            plane* pl = findMainPlane();
            // fuck this line.
            // many months later still fuck this line
            pl->setTeamid(m_teknofestProperties->takimid());
            pl->setGpsSaati(QDateTime::currentDateTime());
            //plane* plane = m_planeController->findPlane(pl->sysid());
            //plane* plane = createDummyPlane();
            QJsonObject data = pl->toJson();
            m_httpClient->sendLocationData(m_teknofestProperties, m_planeController, data);
        }
    }
}

int MavLinkUDP::send_heartbeat()
{
    //qDebug() << "a";
    if(m_mavLinkProperties->connected()){
        mavlink_message_t message;
        mavlink_heartbeat_t heartbeat;
        heartbeat.type = MAV_TYPE_GCS;
        heartbeat.autopilot = MAV_AUTOPILOT_INVALID;
        heartbeat.base_mode = 0;
        heartbeat.custom_mode = 0;
        heartbeat.system_status = 0;

        mavlink_msg_heartbeat_encode(m_mavLinkProperties->sysid(), 200, &message, &heartbeat);


        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        const int len = mavlink_msg_to_send_buffer(buffer, &message);
        if(m_mavLinkProperties->isSerial()){
            //qDebug() << m_serialManager.serialPort()->portName();
            qint64 bytesWritten = m_serialManager.serialPort()->write(reinterpret_cast<const char*>(buffer), len);
            if (bytesWritten != len) {
                qDebug() << "[ERROR] Heartbeat failed to send.";
                return -2;
            } else {
                //qDebug() << "HEARTBEAT message sent!";
                return 0;
            }
        }else {
            int ret = sendto(m_udpManager.socket_fd(), buffer, len, 0, m_udpManager.getSourceAddress(), m_udpManager.src_addr_len());
            if (ret != len) {
                qDebug() << "[ERROR] heartbeat failed to send. \n";
                return -2;
            } else {
                //qDebug() << "HEARTBEAT message sent! \n";
                return 0;
            }
        }


    }else{
        //qDebug() << "[INFO] Socket connection is not set yet! Use open_socket() \n";
        return -1;
    }
    return 0;
}

void MavLinkUDP::armedChanged(bool armed, bool forced){
    //qDebug() << "Status: " << armed;
    // MAV_CMD_COMPONENT_ARM_DISARM 400
    // 0 disarm 1 arm
    if(m_mavLinkProperties->connected()){

        int sysid = m_teknofestProperties->planeids()[0];
        if(m_mavLinkProperties->isSerial()){
            m_serialManager.sendMAVLinkCommand(sysid, MAV_CMD_COMPONENT_ARM_DISARM, armed ? 1 : 0, forced ? 21196 : 0, 0,0,0,0,0);
        }else {
            m_udpManager.sendMAVLinkCommand(sysid, MAV_CMD_COMPONENT_ARM_DISARM, armed ? 1 : 0, forced ? 21196 : 0, 0,0,0,0,0);
        }
        qDebug() << (forced ? "Forcefully" : "Peacefully")
                 << (armed ? "armed" : "disarmed")
                 << "the plane with id: " << sysid << ".";
        QString text;
        text.append(forced ? "Forcefully" : "Peacefully").append(" ");
        text.append(armed ? "armed" : "disarmed").append(" ");
        text.append("the plane with id: ").append(QString::number(sysid)).append(".");
        m_notificationCenter->sendNotification(text, m_notificationCenter->Success);
    }else {
        m_notificationCenter->sendNotification("AvaSYS does not have connected vehicles.", m_notificationCenter->Error);
    }
}

void MavLinkUDP::flyStateChanged(bool isFlying)
{
    if(m_mavLinkProperties->connected()){
        qDebug() << "Fly state change fired";
        int sysid = m_teknofestProperties->planeids()[0];
        if(m_mavLinkProperties->isSerial()){
            m_serialManager.sendMAVLinkCommand(sysid, isFlying ? MAV_CMD_NAV_TAKEOFF : MAV_CMD_NAV_RETURN_TO_LAUNCH, 0,0, 0, 30 ,0,0,0);
        }else {
            if(isFlying){
                m_udpManager.sendMAVLinkCommand(sysid, MAV_CMD_DO_SET_MODE,
                                          MAV_MODE_AUTO_ARMED, 0, 0, 0, 0, 0, 0);
            }else {
                m_udpManager.sendMAVLinkCommand(sysid, MAV_CMD_NAV_RETURN_TO_LAUNCH, 0 ,0, 0, 0,0,0,0);
            }
            flyingStateChanged(isFlying);
            //m_udpManager.sendMAVLinkCommand(sysid, !isFlying ? MAV_CMD_NAV_TAKEOFF : MAV_CMD_NAV_RETURN_TO_LAUNCH, 15 ,0, 0, 30 ,0,0,0);
        }
        m_notificationCenter->sendNotification("Sent takeoff command!", m_notificationCenter->Success);
        qDebug() << "Sent takeoff command!";
        //qDebug() << (forced ? "Forcefully" : "Peacefully")
        //         << (armed ? "armed" : "disarmed")
        //         << "the plane with id: " << sysid << ".";
    }
}



void MavLinkUDP::flyingStateChanged(bool isFlying)
{
    m_mavLinkProperties->setIsFlying(isFlying);
}

NotificationCenter *MavLinkUDP::notificationCenter() const
{
    return m_notificationCenter;
}

TeknofestProperties *MavLinkUDP::teknofestProperties() const
{
    return m_teknofestProperties;
}

plane *MavLinkUDP::findMainPlane()
{
    if (!m_teknofestProperties->planeids().empty()){
        //qDebug() << "found the plane";
        plane* plane = m_planeController->findMainPlane(m_teknofestProperties->planeids()[0]);
        //qDebug() << plane->longitude();
        //qDebug() << plane->gpsSaati();
        return plane;
    }else{
        //qDebug() << "nope";
        return nullptr;
    }
}

std::vector<TeknoCircles *> MavLinkUDP::circles() const
{
    return m_circles;
}

void MavLinkUDP::setCircles(const std::vector<TeknoCircles *> &newCircles)
{
    if (m_circles == newCircles)
        return;
    m_circles = newCircles;
    emit circlesChanged();
}



/// To be removed later.

plane* MavLinkUDP::createDummyPlane(){
    plane* plane = new class plane();
    plane->setSysid(m_teknofestProperties->takimid());
    plane->setAltitude(10);
    plane->setLatitude(-122.25474548339844);
    plane->setLongitude(37.52430725097656);
    plane->setYaw(2);
    /*plane::LocationData locationData;
    locationData.gpsSaati = QDateTime::currentDateTime();
    locationData.hedefGenislik = 0;
    locationData.hedefMerkezX = 0;
    locationData.hedefMerkezY = 0;
    locationData.hedefYukseklik = 0;
    locationData.ihaBatarya = 0;
    locationData.ihaBoylam = -122.25474548339844;
    locationData.ihaDikilme = 0;
    locationData.ihaEnlem = 37.52430725097656;
    locationData.ihaHiz = 0;
    locationData.ihaIrtifa = 10;
    locationData.ihaKilitlenme = false;
    locationData.ihaOtonom = false;
    locationData.ihaYatis = 3;
    locationData.ihaYonelme = plane->yaw();
    locationData.takimNumarasi = m_teknofestProperties->takimid();

    plane->setLocationData(locationData);*/
    return plane;
}
void MavLinkUDP::onDisconnect(int sysid)
{

}
