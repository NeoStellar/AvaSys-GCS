#include "udpmanager.h"
#include "qdebug.h"
#include <arpa/inet.h>
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

QByteArray UDPReceiverThread::convertImageToQByteArray(const mavlink_camera_image_captured_t &imageCaptured)
{
    const uint8_t* imageData = reinterpret_cast<const uint8_t*>(imageCaptured.capture_result);

    // Calculate the size of the image data
    int dataSize = 640 * 640 * 3; // Assuming RGB image

    // Create a QByteArray and copy image data into it
    QByteArray byteArray(reinterpret_cast<const char*>(imageData), dataSize);

    return byteArray;
}

void UDPReceiverThread::run() {
    while (!isInterruptionRequested()) {
        uint8_t buf[MAVLINK_MAX_PACKET_LEN];
        ssize_t recvLen = recv(m_socket_fd, buf, sizeof(buf), 0);
        //ssize_t recvLen = recvfrom(m_socket_fd, buffer, sizeof(buffer), 0,
        //                           nullptr, nullptr); // We don't need sender info here
        // recvLen == -1
        if (false) {
            qDebug() << "Failed to receive data from socket";
            break;
        }

        //qDebug() << "recvLen:" << recvLen;

        mavlink_message_t msg;
        mavlink_status_t status;

        //qDebug() << "handle";



        //QByteArray byteArray(reinterpret_cast<char*>(buf), recvLen);
        QByteArray byteArray(reinterpret_cast<const char*>(&recvLen), recvLen);


        int error = 0;
        if(recvLen != -1){
            for (ssize_t i = 0; i < recvLen; ++i) {
                if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) {
                    //qDebug() << "abc";
                    // Mavlink mesajını filtreleme
                    float vx, vy, vz, speed, pressure;
                    switch (msg.msgid) {

                        case MAVLINK_MSG_ID_SCALED_PRESSURE:
                            //mavlink_highres_imu_t imu_data;
                            //mavlink_msg_highres_imu_decode(&msg, &imu_data);
                            //pressure = imu_data.abs_pressure;
                            //altitude = imu_data.pressure_alt;
                            mavlink_scaled_pressure_t scaled_pressure;
                            mavlink_msg_scaled_pressure_decode(&msg, &scaled_pressure);

                            pressure = scaled_pressure.press_abs;
                            emit pressureDataReceived(msg.sysid, pressure);
                            // Extract barometer data
                            // Print barometer values
                            //qDebug() << "Barometer - Pressure: " << pressure << " bar, Altitude: " << altitude << " m";
                            break;
                        case MAVLINK_MSG_ID_CAMERA_IMAGE_CAPTURED:
                            mavlink_camera_image_captured_t imageCaptured;
                            mavlink_msg_camera_image_captured_decode(&msg, &imageCaptured);
                            //qDebug() << "Received HEARTBEAT message";
                            //qDebug() << "Heartbeat from " << msg.sysid;
                            qDebug() << "Received camera data";
                            emit imageCapturedSignal(640, 640, convertImageToQByteArray(imageCaptured));
                            break;
                        case MAVLINK_MSG_ID_SYS_STATUS:
                            mavlink_sys_status_t sysStatus;
                            mavlink_msg_sys_status_decode(&msg, &sysStatus);
                            emit batteryDataReceived(msg.sysid, sysStatus.voltage_battery / 1000.0f, sysStatus.current_battery / 100.0f, sysStatus.battery_remaining);
                            //mavlink_camera_information_t info;
                            break;
                        case MAVLINK_MSG_ID_COMMAND_ACK:
                            mavlink_command_ack_t ack;
                            mavlink_msg_command_ack_decode(&msg, &ack);
                            if (ack.command == MAV_CMD_COMPONENT_ARM_DISARM && ack.result == MAV_RESULT_DENIED) {
                                // Disarming denied: not landed message received
                                qDebug() << "Disarming denied: not landed";
                                // Handle the message here
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
                            //qDebug() << "heartbeat start";
                            //qDebug () << heartbeat.autopilot;
                            //qDebug () << heartbeat.base_mode;
                            //qDebug () << heartbeat.custom_mode;
                            //qDebug () << heartbeat.mavlink_version;
                            //qDebug () << heartbeat.system_status;
                            //qDebug () << heartbeat.type;
                            //qDebug () << "heartbeat end";

                            bool armed = heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED; //MAV_MODE_FLAG_SAFETY_ARMED;
                            //bool isArmed = heartbeat.base_mode & (MAV_MODE_FLAG_DECODE_POSITION_MANUAL| MAV_MODE_FLAG_DECODE_POSITION_STABILIZE |
                            //                                      MAV_MODE_FLAG_DECODE_POSITION_GUIDED | MAV_MODE_FLAG_DECODE_POSITION_AUTO);
                            // Use 'armed' variable as needed
                            //qDebug() << "Armed status: " << (armed ? "Armed" : "Disarmed");
                            //qDebug() << heartbeat.base_mode;
                            emit armDataReceived(msg.sysid, armed);
                            break;
                        }
                        case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
                            mavlink_local_position_ned_t local_pos;
                            mavlink_msg_local_position_ned_decode(&msg, &local_pos);

                            // Extract speed components
                            vx = local_pos.vx; // North velocity
                            vy = local_pos.vy; // East velocity
                            vz = local_pos.vz; // Down velocity

                            speed = sqrt(vx * vx + vy * vy + vz * vz);
                            // Print received speed
                            //qDebug() << "Received speed: " << speed << " m/s";
                            emit airspeedDataReceived(msg.sysid, speed);
                            break;
                        case MAVLINK_MSG_ID_ATTITUDE:
                            mavlink_attitude_t attitude;
                            mavlink_msg_attitude_decode(&msg, &attitude);


                            // Extract yaw (heading) from the received ATTITUDE message
                            //attitude.yaw;
                            //qDebug() << "Yaw [" << msg.sysid << "]: " << attitude.yaw;
                            //emit yawDataRecieved(msg.sysid, attitude.yaw);
                            // Process yaw data as needed
                            break;
                        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
                            //qDebug() << "Got coordinate data from " << msg.sysid;
                            mavlink_global_position_int_t gps_data;
                            mavlink_msg_global_position_int_decode(&msg, &gps_data);

                            //if(msg.sysid == 1){
                            //    break;
                            //}

                            //qDebug() << "SYS ID : " << msg.sysid;

                            int32_t latitude = gps_data.lat;  // Latitude in degrees * 1e7
                            int32_t longitude = gps_data.lon; // Longitude in degrees * 1e7
                            int32_t altitude = gps_data.alt;  // Altitude in millimeters
                            //qDebug() << "hdg:" << gps_data.hdg / 100;
                            float fLat = latitude / 1e7;
                            float fLong = longitude / 1e7;
                            int32_t fAlt = altitude / 1000;
                            emit locationDataRecieved(msg.sysid, fLat, fLong, fAlt);
                            emit yawDataRecieved(msg.sysid, gps_data.hdg / 100);

                            // Convert latitude, longitude, and altitude to appropriate units if necessary
                            // For example, divide by 1e7 to convert latitude and longitude to degrees
                            // Divide altitude by 1000 to convert millimeters to meters

                            //qDebug() << "Latitude: " << fLat;
                            //qDebug() << "Longitude: " << fLong;
                            //qDebug() << "Altitude: " << fAlt;
                            break;

                    }
                }
            }
            emit dataReceived(byteArray);
        }else{
            qDebug() << "Connection problem! Error is: " << error;
            error += 1;
            if(error >= 5){
                qDebug() << "Connection LOST closing socket";
                emit dataReceived(byteArray);

                //close(socket_fd());
                qDebug() << "Socket closed successfully";
                return;
            }else{
                QThread::sleep(1);
            }
        }
    }
}

ssize_t extractSSizeT(const QByteArray &byteArray, int startIndex) {
    // Check if there are enough bytes in the array to represent ssize_t
    if (startIndex + static_cast<int>(sizeof(ssize_t)) > byteArray.size()) {
        qDebug() << "Not enough bytes in the array to represent ssize_t";
        return -1; // Or any suitable error handling
    }

    // Interpret the bytes as ssize_t
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
    m_receiverThread->start();

    m_mavlinkProperties->setSysid(1);

    // Create a timer to check for received data periodically (optional)
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        if (!m_receiverThread->isRunning()) {
            qDebug() << "Receiver thread is not running!";
            stop(); // Stop if receiver thread is not running
            m_mavlinkProperties->setConnected(false);
        }
    });
    m_timer->start(1000); // Adjust the interval as needed

    emit connected();
    // Assume HeartbeatListener and startHeartbeat() methods are defined elsewhere
    // HeartbeatListener *listener = new HeartbeatListener(m_socket_fd, &m_is_connected);
    // listener->start();
    // startHeartbeat();
    return 1;
}

void UDPManager::connected(){
    qDebug() << "UDP MANAGER signal";
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

void UDPManager::onDisconnect()
{
    qDebug() << "Disconnected!";
}

bool UDPManager::src_addr_set() const
{
    return m_src_addr_set;
}

void UDPManager::setSrc_addr_set(bool newSrc_addr_set)
{
    m_src_addr_set = newSrc_addr_set;
}

void UDPManager::onReadyRead() {

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
