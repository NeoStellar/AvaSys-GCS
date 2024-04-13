#include "udpmanager.h"
#include "qdebug.h"
#include <arpa/inet.h>
#include <QBuffer>
#include <QImage>
#include <unistd.h>
#include "mavlinkproperties.h"
#include "mavlink-out/common/mavlink.h"

UDPReceiverThread::UDPReceiverThread(int socket_fd, QObject *parent) : QThread(parent), m_socket_fd(socket_fd)
{

}

UDPReceiverThread::~UDPReceiverThread() {
    quit();
    wait();
}

QByteArray createBlackScreenByteArray(int width, int height) {
    QImage blackScreen(width, height, QImage::Format_RGB888);
    blackScreen.fill(Qt::darkBlue);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    blackScreen.save(&buffer, "PNG");

    return byteArray;
}

QByteArray UDPReceiverThread::convertImageToQByteArray(const mavlink_camera_image_captured_t &imageCaptured)
{

    if(imageCaptured.capture_result != 0){
        // trying to fix the sigsegv, not working tho
        return createBlackScreenByteArray(640,640);
    }
    const uint8_t* imageData = reinterpret_cast<const uint8_t*>(imageCaptured.capture_result);

    if(imageData == nullptr){
        qDebug() << "adsacu";
    }
    if(imageData == NULL){
        qDebug() << "nul";
    }

    int dataSize = 640 * 640 * 3; // Assuming RGB image

    // this fucker gives me segmentation fault. idk how. idk why. its fucking RANDOM WHY THE FUCK?
    // *unsegments your fault*
    QByteArray byteArray(reinterpret_cast<const char*>(imageData), dataSize);

    return byteArray;
}

void UDPReceiverThread::run() {
    while (!isInterruptionRequested()) {
        uint8_t buf[MAVLINK_MAX_PACKET_LEN];
        ssize_t recvLen = recv(m_socket_fd, buf, sizeof(buf), 0);
        if (false) {
            qDebug() << "Failed to receive data from socket";
            break;
        }

        //qDebug() << "recvLen:" << recvLen;

        mavlink_message_t msg;
        mavlink_status_t status;

        QByteArray byteArray(reinterpret_cast<const char*>(&recvLen), recvLen);
        if(recvLen != -1){
            for (ssize_t i = 0; i < recvLen; ++i) {
                if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) {
                    float vx, vy, vz, speed, pressure;
                    switch (msg.msgid) {
                        case MAVLINK_MSG_ID_SCALED_PRESSURE:
                            mavlink_scaled_pressure_t scaled_pressure;
                            mavlink_msg_scaled_pressure_decode(&msg, &scaled_pressure);

                            pressure = scaled_pressure.press_abs;
                            emit pressureDataReceived(msg.sysid, pressure);
                            // Extract barometer data
                            //qDebug() << "Barometer - Pressure: " << pressure << " bar, Altitude: " << altitude << " m";
                            break;
                        case MAVLINK_MSG_ID_CAMERA_IMAGE_CAPTURED:
                            mavlink_camera_image_captured_t imageCaptured;
                            mavlink_msg_camera_image_captured_decode(&msg, &imageCaptured);
                            emit imageCapturedSignal(640, 640, convertImageToQByteArray(imageCaptured));
                            break;
                        case MAVLINK_MSG_ID_SYS_STATUS:
                            mavlink_sys_status_t sysStatus;
                            mavlink_msg_sys_status_decode(&msg, &sysStatus);
                            emit batteryDataReceived(msg.sysid, sysStatus.voltage_battery / 1000.0f, sysStatus.current_battery / 100.0f, sysStatus.battery_remaining);
                            break;
                        case MAVLINK_MSG_ID_COMMAND_ACK:
                            mavlink_command_ack_t ack;
                            mavlink_msg_command_ack_decode(&msg, &ack);
                            if (ack.command == MAV_CMD_COMPONENT_ARM_DISARM && ack.result == MAV_RESULT_DENIED) {
                                // Disarming denied: not landed message received
                                qDebug() << "Disarming denied: not landed";
                                // Handle the message here
                            }
                            if(ack.command == MAV_CMD_NAV_TAKEOFF){
                                emit flyingStateChanged(ack.result == MAV_RESULT_ACCEPTED);
                            }
                            break;
                        case MAVLINK_MSG_ID_VIDEO_STREAM_STATUS:
                            mavlink_video_stream_status_t deneme;
                            mavlink_msg_video_stream_status_decode(&msg, &deneme);
                            qDebug() << "flags: " << deneme.flags;
                            break;
                        case MAVLINK_MSG_ID_HEARTBEAT: {
                            mavlink_heartbeat_t heartbeat;
                            mavlink_msg_heartbeat_decode(&msg, &heartbeat);

                            // idk why and how it gives ghost heartbeat, but it is hacky fix.
                            if(heartbeat.type == 0){
                                break;
                            }

                            bool armed = heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED; //MAV_MODE_FLAG_SAFETY_ARMED;
                            //bool isArmed = heartbeat.base_mode & (MAV_MODE_FLAG_DECODE_POSITION_MANUAL| MAV_MODE_FLAG_DECODE_POSITION_STABILIZE |
                            //                                      MAV_MODE_FLAG_DECODE_POSITION_GUIDED | MAV_MODE_FLAG_DECODE_POSITION_AUTO);
                            //qDebug() << "Armed status: " << (armed ? "Armed" : "Disarmed");
                            //qDebug() << heartbeat.base_mode;
                            emit armDataReceived(msg.sysid, armed);
                            break;
                        }
                        case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
                            mavlink_local_position_ned_t local_pos;
                            mavlink_msg_local_position_ned_decode(&msg, &local_pos);
                            vx = local_pos.vx;
                            vy = local_pos.vy;
                            vz = local_pos.vz;

                            speed = sqrt(vx * vx + vy * vy + vz * vz);
                            //qDebug() << "Received speed: " << speed << " m/s";
                            emit airspeedDataReceived(msg.sysid, speed);
                            break;
                        case MAVLINK_MSG_ID_ATTITUDE:
                            mavlink_attitude_t attitude;
                            mavlink_msg_attitude_decode(&msg, &attitude);
                            emit attitudeDataReceived(msg.sysid, attitude.roll, attitude.pitch, attitude.yaw);
                            //qDebug() << "Yaw [" << msg.sysid << "]: " << attitude.yaw;
                            //emit yawDataRecieved(msg.sysid, attitude.yaw);
                            break;
                        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
                            //qDebug() << "Got coordinate data from " << msg.sysid;
                            mavlink_global_position_int_t gps_data;
                            mavlink_msg_global_position_int_decode(&msg, &gps_data);

                            int32_t latitude = gps_data.lat;  // Latitude in degrees * 1e7
                            int32_t longitude = gps_data.lon; // Longitude in degrees * 1e7
                            int32_t altitude = gps_data.alt;  // Altitude in millimeters

                            float fLat = latitude / 1e7;
                            float fLong = longitude / 1e7;
                            int32_t fAlt = altitude / 1000;

                            emit locationDataRecieved(msg.sysid, fLat, fLong, fAlt);
                            emit yawDataRecieved(msg.sysid, gps_data.hdg / 100);

                            // !!
                            // this one is for real life situations, not suitable for simulations.
                            // !!

                            // Check if altitude is above a certain threshold (e.g., 2 meters)
                            const float MIN_ALTITUDE_AGL = 2.0f;
                            if (fAlt > MIN_ALTITUDE_AGL) {
                                // Vehicle is above minimum altitude threshold
                                // Check if the vertical velocity is positive (indicating ascent)
                                //emit flyingStateChanged(gps_data.vz > 0);
                            } else {
                                // Altitude is below minimum threshold, vehicle is likely on the ground
                                //emit flyingStateChanged(false);
                            }
                            break;

                    }
                }
            }
            emit dataReceived(byteArray);
        }else{
            this->terminate();
            QThread::wait();
            // i really do not know why error integer was not working. just remove it. what can go wrong?
            //qDebug() << "Connection problem! Error is: " << error;
            //error += 1;
            /*if(error >= 5){
                qDebug() << "Connection LOST closing socket";
                //emit dataReceived(byteArray);

                //close(socket_fd());
                qDebug() << "Socket closed successfully";
                return;
            }else{
                QThread::sleep(1);
            }*/
        }
    }
}

ssize_t extractSSizeT(const QByteArray &byteArray, int startIndex) {
    // Check if there are enough bytes in the array to represent ssize_t
    if (startIndex + static_cast<int>(sizeof(ssize_t)) > byteArray.size()) {
        qDebug() << "Not enough bytes in the array to represent ssize_t";
        return -1; // Or any suitable error handling
    }
    ssize_t value;
    memcpy(&value, byteArray.constData() + startIndex, sizeof(ssize_t));
    return value;
}




UDPManager::UDPManager(MavLinkProperties *mavlinkProperties, QObject *parent)
    : QObject(parent),
    m_socket_fd(-1),
    m_src_addr_set(false),
    m_mavlinkProperties(mavlinkProperties)
{
    qRegisterMetaType<int32_t>("int32_t");
}

UDPManager::~UDPManager() {
    stop();
}
const sockaddr* UDPManager::getSourceAddress() const {
    return reinterpret_cast<const sockaddr*>(&m_src_addr);
}

void UDPManager::sendMAVLinkCommand(int sysid, uint16_t command, float param1, float param2, float param3, float param4, float param5, float param6, float param7)
{
    mavlink_message_t msg;
    mavlink_msg_command_long_pack(sysid, 200, &msg,
                                  1, 1, // Target System and Component
                                  command, 0, param1, param2, param3, param4, param5, param6, param7);
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);


    int cu = sendto(m_socket_fd, buf, len, 0, (const struct sockaddr*) getSourceAddress(), m_src_addr_len);
    // Send the buffer using sendto
    if (cu != len) {
        qDebug() << "Failed to send MAVLink command via UDP";
    }
}

int UDPManager::initialize(const QString& ipString, int port) {
    m_socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (m_socket_fd < 0) {
        qDebug() << "Socket creation failed.";
        emit errorOccurred(-1);
        m_mavlinkProperties->setConnected(false);
        return -1;
    }

    qDebug() << "Socket created. Socketfd is" << m_socket_fd;
    qDebug() << "IP:" << ipString;
    qDebug() << "Port:" << port;

    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ipString.toStdString().c_str(), &m_addr.sin_addr);
    m_addr.sin_port = htons(port);

    if (bind(m_socket_fd, (struct sockaddr*)(&m_addr), sizeof(m_addr)) != 0) {
        qDebug() << "Socket bind failed.";
        emit errorOccurred(-2);
        m_mavlinkProperties->setConnected(false);
        return -2;
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    if (setsockopt(m_socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        qDebug() << "Socket option setting failed.";
        emit errorOccurred(-4);
        m_mavlinkProperties->setConnected(false);
        return -4;
    }

    memset(&m_src_addr, 0, sizeof(m_src_addr));
    m_src_addr_len = sizeof(m_src_addr);
    m_src_addr_set = false;

    char buffer[2048]; // enough for MTU 1500 bytes

    const int ret = recvfrom(m_socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)(&m_src_addr), &m_src_addr_len);

    if (ret < 0) {
        qDebug() << "Socket receive failed.";
        emit errorOccurred(-3);
        close(m_socket_fd);
        m_mavlinkProperties->setConnected(false);
        return -3;
    } else if (ret == 0) {
        qDebug() << "Peer has done an orderly shutdown.";
        emit errorOccurred(-3);
        m_mavlinkProperties->setConnected(false);
        return -3;
    }

    m_src_addr_set = true;


    // Start receiving data in a separate thread
    m_receiverThread = new UDPReceiverThread(m_socket_fd, this);
    connect(m_receiverThread, &UDPReceiverThread::dataReceived, this, &UDPManager::handleReceivedData);
    connect(m_receiverThread, &UDPReceiverThread::locationDataRecieved, this, &UDPManager::locationDataRecieved);
    connect(m_receiverThread, &UDPReceiverThread::yawDataRecieved, this, &UDPManager::yawDataRecieved);
    connect(m_receiverThread, &UDPReceiverThread::airspeedDataReceived, this, &UDPManager::airspeedDataReceived);
    connect(m_receiverThread, &UDPReceiverThread::pressureDataReceived, this, &UDPManager::pressureDataReceived);
    connect(m_receiverThread, &UDPReceiverThread::imageCapturedSignal, this, &UDPManager::imageCapturedSignal);
    connect(m_receiverThread, &UDPReceiverThread::batteryDataReceived, this, &UDPManager::batteryDataReceived);
    connect(m_receiverThread, &UDPReceiverThread::armDataReceived, this, &UDPManager::armDataReceived);
    connect(m_receiverThread, &UDPReceiverThread::flyingStateChanged, this, &UDPManager::flyingStateChanged);
    connect(m_receiverThread, &UDPReceiverThread::attitudeDataReceived, this, &UDPManager::attitudeDataReceived);
    m_receiverThread->start();

    m_mavlinkProperties->setSysid(1);

    // Create a timer to check for received data periodically (optional)
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        if (!m_receiverThread->isRunning()) {
            qDebug() << "Receiver thread is not running!";
            stop(); // Stop if receiver thread is not running
            m_mavlinkProperties->setConnected(false);
            emit onDisconnect(m_mavlinkProperties->sysid());
        }
    });
    m_timer->start(1000); // Adjust the interval as needed

    //emit connected();
    // Assume HeartbeatListener and startHeartbeat() methods are defined elsewhere
    // HeartbeatListener *listener = new HeartbeatListener(m_socket_fd, &m_is_connected);
    // listener->start();
    // startHeartbeat();
    return 1;
}
void UDPManager::stop() {
    if (m_receiverThread) {
        m_receiverThread->requestInterruption();
        m_receiverThread->wait();
        delete m_receiverThread;
        m_receiverThread = nullptr;
    }

    if (m_timer) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }

    if (m_socket_fd != -1) {
        close(m_socket_fd);
        m_socket_fd = -1;
    }
    m_mavlinkProperties->setConnected(false);
}

void UDPManager::handleReceivedData(const QByteArray &data) {
    ssize_t extractedValue = extractSSizeT(data, 0);
    //qDebug() << "Extracted value: " << extractedValue;
    if(extractedValue == -1){
        qDebug() << "failed attempt";
        m_mavlinkProperties->setConnected(false);
        stop();
    }else {
    }

}

void UDPManager::armDataReceived(int sysid, bool armed){
    m_mavlinkProperties->setArmed(armed);
}
void UDPManager::onErrorOccurred(int errorCode)
{
    qDebug() << &"Error: " [ errorCode];
}

void UDPManager::onReadyRead()
{

}

bool UDPManager::src_addr_set() const
{
    return m_src_addr_set;
}

void UDPManager::setSrc_addr_set(bool newSrc_addr_set)
{
    m_src_addr_set = newSrc_addr_set;
}

socklen_t UDPManager::src_addr_len() const
{
    return m_src_addr_len;
}

int UDPManager::socket_fd() const
{
    return m_socket_fd;
}

sockaddr_in UDPManager::src_addr() const
{
    return m_src_addr;
}

void UDPManager::setSrc_addr(const sockaddr_in &newSrc_addr)
{
    m_src_addr = newSrc_addr;
}
