#include "mavlinkudp.h"
#include "mavlink-out/common/mavlink.h"
#include "qdebug.h"
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <unistd.h>
#include <utils/httpclient.h>

MavLinkUDP::MavLinkUDP(MavLinkProperties *mavlinkProperties, PlaneController *planeController, TeknofestProperties *teknofestProperties, HttpClient *httpClient, QObject *parent)
    : QObject(parent),
    m_udpManager(mavlinkProperties),
    m_mavLinkProperties(mavlinkProperties),
    m_planeController(planeController),
    m_teknofestProperties(teknofestProperties),
    m_httpClient(httpClient)
{
    qRegisterMetaType<int32_t>("int32_t");
    // Connect signals from MavLinkProperties to corresponding slots
    connect(m_mavLinkProperties, &MavLinkProperties::armedChanged, this, &MavLinkUDP::onArmedChangedSlot);

    // Configure heartbeat timer
    //connect(&m_heartbeatTimer, &QTimer::timeout, this, &MavLinkUDP::onHeartbeatTimeout);
    //m_heartbeatTimer.start(1000);



     // Adjust interval as needed (in milliseconds)



    //connect(m_sendHeartbeatThread, &QThread::started, this, &MavLinkUDP::sendHeartbeat);

}
MavLinkUDP::~MavLinkUDP() {
}

int MavLinkUDP::initialize(const QString& ipString, int port) {
    int result = m_udpManager.initialize(ipString, port);
    if (result < 0) {
        // Forward error to parent
        emit errorOccurred(result);
        return result;
    }

    // Connect signals from UDPManager to MavLink
    connect(&m_udpManager, &UDPManager::connected, this, &MavLinkUDP::connected);
    connect(&m_udpManager, &UDPManager::errorOccurred, this, &MavLinkUDP::errorOccurred);
    connect(&m_udpManager, &UDPManager::locationDataRecieved, this, &MavLinkUDP::locationDataRecieved);
    connect(&m_udpManager, &UDPManager::yawDataRecieved, this, &MavLinkUDP::yawDataRecieved);
    connect(&m_udpManager, &UDPManager::airspeedDataReceived, this, &MavLinkUDP::airspeedDataReceived);
    connect(&m_udpManager, &UDPManager::pressureDataReceived, this, &MavLinkUDP::pressureDataReceived);
    connect(&m_udpManager, &UDPManager::imageCapturedSignal, this, &MavLinkUDP::imageCapturedSignal);



    // Connect signals from MavLinkProperties to MavLink
    connect(m_mavLinkProperties, &MavLinkProperties::connectedChanged, this, &MavLinkUDP::connectedChanged);

    //connect(&m_udpManager, &UDPManager::dataReceived, this, &MavLinkUDP::handleReceivedData);


    m_mavLinkProperties->setConnected(true);

    m_mavLinkProperties->setSysid(m_teknofestProperties->takimid());

    connect(&m_heartbeatTimer, &QTimer::timeout, this
            , &MavLinkUDP::sendHeartbeat);
    m_heartbeatTimer.start(1000);


    //m_planeController->addOrUpdatePlane(1, 37.52, -122.276, 10);
    //m_planeController->addOrUpdatePlane(3, 37.52, -122.276, 10);
    //qDebug() << m_planeController->planes();
    //qDebug() << m_planeController->planes().at(0)->latitude();
    //qDebug() << "Added plane!sa sa";
    return 1;
}
/*void MavLinkUDP::handleReceivedData(const ssize_t &data) {

} */

void MavLinkUDP::connected(){
    //m_planeController->addPlane(1, 37.52, -122.276, 10);
    qDebug() << "A!";
}
template <typename T>
bool contains(
    const std::vector<T>& vecObj,
    const T& element)
{
    // Get the iterator of first occurrence
    // of given element in vector
    auto it = std::find(
        vecObj.begin(),
        vecObj.end(),
        element) ;
    return it != vecObj.end();
}
void MavLinkUDP::locationDataRecieved(int sysid, float latitude, float longitude, int32_t altitude){
    //qDebug() << "latitude: " << latitude;
    //qDebug() << "longitude: " << longitude;
    //qDebug() << "altitude: " << altitude;
    //qDebug() << "Got coordinate data from " << sysid;
    if(!m_teknofestProperties->simMode()){
        m_planeController->setTeam(sysid, -1);
    }else {
        m_planeController->setTeam(sysid, m_teknofestProperties->takimid());
    }
    m_planeController->AoULocalPlane(sysid, latitude, longitude, altitude);
    //qDebug() << "locdata: " << m_planeController->findPlane(sysid)->teamid();
}

void MavLinkUDP::yawDataRecieved(int sysid, float yaw){
    m_planeController->updateLocalYaw(sysid, yaw);
}
void MavLinkUDP::airspeedDataReceived(int id, float speed){
    m_planeController->updateLocalSpeed(id, speed);
}
void MavLinkUDP::pressureDataReceived(int id, float pressure){
    m_planeController->updateLocalPressure(id, pressure);
}


void MavLinkUDP::onHeartbeatTimeout() {
    sendHeartbeat();
}

void MavLinkUDP::sendHeartbeat() {

    //qDebug() << "Sending heartbeat started";
    // Implement sending heartbeat message using m_udpManager
    // Example:
    // m_udpManager.sendData(heartbeatData);
    if(m_mavLinkProperties->connected()){
        // Kalp atışı gönderme işlemi

        send_heartbeat();
        plane* pl = m_planeController->planes()[0];
        pl->setTeamid(m_teknofestProperties->takimid());
        pl->setGpsSaati(QDateTime::currentDateTime());
        //plane* plane = m_planeController->findPlane(pl->sysid());
        //plane* plane = createDummyPlane();
        QJsonObject data = pl->toJson();
        m_httpClient->sendLocationData("http://replica.neostellar.net/api/telemetri_gonder", m_teknofestProperties, m_planeController, data);
    }
}
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

        mavlink_msg_heartbeat_encode(1, 200, &message, &heartbeat);


        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        const int len = mavlink_msg_to_send_buffer(buffer, &message);
        int ret = sendto(m_udpManager.socket_fd(), buffer, len, 0, m_udpManager.getSourceAddress(), m_udpManager.src_addr_len());
        if (ret != len) {
            //qDebug() << "[ERROR] sendto error \n";
            return -2;
        } else {
            //qDebug() << "HEARTBEAT message sent! \n";
            return 0;
        }

    }else{
        //qDebug() << "[INFO] Socket connection is not set yet! Use open_socket() \n";
        return -1;
    }
    return 0;
}





void MavLinkUDP::onArmedChangedSlot(bool armed) {
    // Forward the armed state change signal
    emit armedChanged(armed);
}

void MavLinkUDP::armedChanged(bool armed){
    qDebug() << "Status: " << armed;
    // MAV_CMD_COMPONENT_ARM_DISARM 400
    // 0 disarm 1 arm
    if(m_mavLinkProperties->connected()){
        mavlink_message_t arm_message;
        mavlink_command_long_t armT;
        armT.target_system = 1;
        armT.target_component = 1;
        armT.command = MAV_CMD_COMPONENT_ARM_DISARM;
        armT.confirmation = 1;
        armT.param1 = armed ? 1 : 0;
        mavlink_msg_command_long_encode(1, 200, &arm_message, &armT);
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        const int len = mavlink_msg_to_send_buffer(buffer, &arm_message);
        int cu = sendto(m_udpManager.socket_fd(), buffer, len, 0, (const struct sockaddr*) m_udpManager.getSourceAddress(), m_udpManager.src_addr_len());
        if (cu != len){
            qDebug() << "eşit değil";
            return;
        }
        qDebug() << cu;
    }
}

TeknofestProperties *MavLinkUDP::teknofestProperties() const
{
    return m_teknofestProperties;
}
