#include "serialmanager.h"
#include "qdebug.h"
#include <QSerialPort>
#define MAV_RAD_TO_DEG(x) ((x) * 180.0f / M_PI)

ssize_t extractSSizeT2(const QByteArray &byteArray, int startIndex) {
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


SerialManager::SerialManager(MavLinkProperties *mavlinkProperties, QObject *parent)
    : QObject(parent), m_mavlinkProperties(mavlinkProperties)
{
    m_serialPort = new QSerialPort(this);
    /*connect(&m_serialPort, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error){
        emit errorOccurred(m_serialPort->errorString());
    });*/
}

SerialManager::~SerialManager()
{

}

void SerialManager::handle(){

    if (!m_serialPort || !m_serialPort->isOpen()) {
        qDebug() << "Serial port is not open or is null.";
        return;
    }

    if (m_serialPort->bytesAvailable() <= 0) {
        //msleep(10); // Sleep briefly to prevent busy-waiting
        qDebug() << "aaskdjak";
        return;
    }
    QByteArray data = m_serialPort->readAll();
    if (data.isEmpty()) {
        qDebug() << "Failed to read data from serial port.";
        return;
    }
    //qDebug() << "Received data:" << data.toHex();
    //uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    //ssize_t recvLen = data.size();
    mavlink_message_t msg;
    mavlink_status_t status;
    for (int i = 0; i < data.size(); ++i) {
        if (mavlink_parse_char(MAVLINK_COMM_0, data.at(i), &msg, &status)) {
            // Message parsed successfully
            float vx, vy, vz, speed, pressure;
            switch (msg.msgid) {
                case MAVLINK_MSG_ID_SCALED_PRESSURE:
                    mavlink_scaled_pressure_t scaled_pressure;
                    mavlink_msg_scaled_pressure_decode(&msg, &scaled_pressure);

                    pressure = scaled_pressure.press_abs;
                    emit pressureDataReceived(msg.sysid, pressure);

                    //qDebug() << "Barometer - Pressure: " << pressure << " bar, Altitude: " << altitude << " m";
                    break;
                case MAVLINK_MSG_ID_CAMERA_IMAGE_CAPTURED:
                    mavlink_camera_image_captured_t imageCaptured;
                    mavlink_msg_camera_image_captured_decode(&msg, &imageCaptured);

                    // there is no camera feature in serial yet.
                    //qDebug() << "Received camera data";
                    //emit imageCapturedSignal(640, 640, convertImageToQByteArray(imageCaptured));
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
                    // Use 'armed' variable as needed
                    //qDebug() << "Armed status: " << (armed ? "Armed" : "Disarmed");
                    //qDebug() << heartbeat.base_mode;
                    //TODO non signal
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


                    // // no longer needed, at least not for now
                    //qDebug() << "Yaw [" << msg.sysid << "]: " << attitude.yaw;
                    //emit yawDataRecieved(msg.sysid, -1 * MAV_RAD_TO_DEG(attitude.yaw));
                    break;
                case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
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
}

int SerialManager::initialize(const QString &serialPort, int baudrate)
{
    qDebug() << m_serialPort;
    m_serialPort->setPortName(serialPort);
    m_serialPort->setBaudRate(baudrate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setReadBufferSize(280); // MAVLINK_MAX_PACKET_LEN
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialManager::handle);

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open serial port:" << m_serialPort->errorString();
    } else {
        qDebug() << "Serial port opened successfully.";
    }


    m_mavlinkProperties->setSysid(1);

    /*
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        if (!m_receiverThread->isRunning()) {
            qDebug() << "Receiver thread is not running!";
            stop(); // Stop if receiver thread is not running
            m_mavlinkProperties->setConnected(false);
        }
    });
    m_timer->start(1000); */

    emit connected();
    return 1;
}

void SerialManager::stop()
{

    if (m_timer) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
    m_mavlinkProperties->setConnected(false);
}

void SerialManager::armDataReceived(int sysid, bool armed){
    m_mavlinkProperties->setArmed(armed);
}

QSerialPort *SerialManager::serialPort() const
{
    return m_serialPort;
}

void SerialManager::setSerialPort(QSerialPort *newSerialPort)
{
    m_serialPort = newSerialPort;
}

void SerialManager::sendMAVLinkCommand(int sysid, uint16_t command, float param1, float param2, float param3, float param4, float param5, float param6, float param7)
{
    mavlink_message_t msg;
    mavlink_msg_command_long_pack(sysid, 200, &msg,
                                  1, 1, // Target System and Component
                                  command, 1, param1, param2, param3, param4, param5, param6, param7);
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

    // Write the buffer to the serial port
    m_serialPort->write(reinterpret_cast<char *>(buf), len);
    m_serialPort->flush();
}

QByteArray SerialManager::convertImageToQByteArray(const mavlink_camera_image_captured_t &imageCaptured)
{
    const uint8_t* imageData = reinterpret_cast<const uint8_t*>(imageCaptured.capture_result);

    // Calculate the size of the image data
    int dataSize = 640 * 640 * 3; // Assuming RGB image

    // Create a QByteArray and copy image data into it
    QByteArray byteArray(reinterpret_cast<const char*>(imageData), dataSize);

    return byteArray;
}



