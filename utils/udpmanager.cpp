#include "udpmanager.h"
#include "qdebug.h"
#include <arpa/inet.h>
#include <unistd.h>
#include "mavlinkproperties.h"
#include "mavlink-out/common/mavlink.h"

UDPReceiverThread::UDPReceiverThread(int socket_fd, QObject *parent) : QThread(parent), m_socket_fd(socket_fd) {}

UDPReceiverThread::~UDPReceiverThread() {
    quit();
    wait();
}

void UDPReceiverThread::run() {
    char buffer[2048]; // Adjust buffer size as needed

    while (!isInterruptionRequested()) {
        ssize_t recvLen = recvfrom(m_socket_fd, buffer, sizeof(buffer), 0,
                                   nullptr, nullptr); // We don't need sender info here
        if (recvLen == -1) {
            qDebug() << "Failed to receive data from socket";
            break;
        }

        // Process received data
        QByteArray data(buffer, recvLen);
        emit dataReceived(data);
    }
}



UDPManager::UDPManager(MavLinkProperties *mavlinkProperties, QObject *parent)
    : QObject(parent),
    m_socket_fd(-1),
    m_src_addr_set(false),
    m_mavlinkProperties(mavlinkProperties)
{
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
    emit connected();

    // Start receiving data in a separate thread
    m_receiverThread = new UDPReceiverThread(m_socket_fd, this);
    connect(m_receiverThread, &UDPReceiverThread::dataReceived, this, &UDPManager::handleReceivedData);
    m_receiverThread->start();

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
    // Process received data here
    //qDebug() << "Received data:" << data;


    mavlink_message_t msg;
    mavlink_status_t status;

    //qDebug() << "handle";

    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    //ssize_t recvLen = recv(socket_fd(), buf, sizeof(buf), 0);
    ssize_t recvLen = data.toLong();
    int error = 0;
    if(recvLen != -1){
        for (ssize_t i = 0; i < recvLen; ++i) {
            if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) {
                // Mavlink mesajını filtreleme
                switch (msg.msgid) {
                case MAVLINK_MSG_ID_HEARTBEAT:
                    //qDebug() << "Received HEARTBEAT message";
                    break;
                case MAVLINK_MSG_ID_SYS_STATUS:
                    // Handle system status message
                    //qDebug() << "Received SYS_STATUS message";
                    break;
                }
            }
        }
    }else{
        qDebug() << "Connection problem! Error is: " << error;
        error += 1;
        if(error >= 5){
            qDebug() << "Connection LOST closing socket";
            m_mavlinkProperties->setConnected(false);
            stop();
            close(socket_fd());
            qDebug() << "Socket closed successfully";
            return;
        }else{
            QThread::sleep(1);
        }
    }
}

void UDPManager::onConnected()
{
    qDebug() << "Connected!";
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
