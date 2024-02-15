#include "mavlinkudp.h"
#include "mavlink-out/common/mavlink.h"
#include "qdebug.h"
#include <QTimer>
#include <unistd.h>

MavLinkUDP::MavLinkUDP(MavLinkProperties *mavlinkProperties, QObject *parent)
    : QObject(parent), m_udpManager(mavlinkProperties),
    m_mavLinkProperties(mavlinkProperties) {

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

    // Connect signals from MavLinkProperties to MavLink
    connect(m_mavLinkProperties, &MavLinkProperties::connectedChanged, this, &MavLinkUDP::connectedChanged);

    connect(&m_udpManager, &UDPManager::dataReceived, this, &MavLinkUDP::handleReceivedData);


    m_mavLinkProperties->setConnected(true);

    connect(&m_heartbeatTimer, &QTimer::timeout, this
            , &MavLinkUDP::sendHeartbeat);
    m_heartbeatTimer.start(1000);

    return 1;
}
void MavLinkUDP::handleReceivedData(const QByteArray &data) {

}


void MavLinkUDP::onHeartbeatTimeout() {
    sendHeartbeat();
}
void MavLinkUDP::receiveHeartbeat() {
    // Implement receiving heartbeat message using m_udpManager
    // Example:
    //connect(&m_udpManager, &UDPManager::dataReceived, this, &MavLinkUDP::handleReceivedData);
    // where handleReceivedData processes incoming data and emits heartbeatReceived() signal if heartbeat message is received
    bool a = true;
    qDebug() << "Recieved something";
    while(a){
        mavlink_message_t msg;
        mavlink_status_t status;

        int error = 0;

        while (true) {
            uint8_t buf[MAVLINK_MAX_PACKET_LEN];
            ssize_t recvLen = recv(m_udpManager.socket_fd(), buf, sizeof(buf), 0);

            if(recvLen != -1){
                error = 0;
                for (ssize_t i = 0; i < recvLen; ++i) {
                    if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) {
                        // Mavlink mesajını filtreleme
                        switch (msg.msgid) {
                        case MAVLINK_MSG_ID_HEARTBEAT:
                            qDebug() << "Received HEARTBEAT message";
                            emit heartbeatReceived();
                            break;
                        }
                    }
                }
            }else{
                qDebug() << "Connection problem! Error is: " << error;
                error++;
                if(error == 5){
                    qDebug() << "Connection LOST closing socket";
                    m_mavLinkProperties->setConnected(false);
                    close(m_udpManager.socket_fd());
                    qDebug() << "Socket closed successfully";
                    a = false;
                    break;
                }else{
                    QThread::sleep(1);
                }
            }
        }
    }
}

void MavLinkUDP::sendHeartbeat() {

    //qDebug() << "Sending heartbeat started";
    // Implement sending heartbeat message using m_udpManager
    // Example:
    // m_udpManager.sendData(heartbeatData);
    if(m_mavLinkProperties->connected()){
        // Kalp atışı gönderme işlemi
        send_heartbeat();
    }


}

int MavLinkUDP::send_heartbeat()
{
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
