#ifndef UDPMANAGER_H
#define UDPMANAGER_H

#include "mavlink-out/common/mavlink.h"
#include "mavlinkproperties.h"

#include <QObject>
#include <QThread>
#include <QTimer>
#include <netinet/in.h>


class UDPReceiverThread : public QThread {

    Q_OBJECT
public:
    explicit UDPReceiverThread(int socket_fd, QObject *parent = nullptr);
    ~UDPReceiverThread();
    QByteArray convertImageToQByteArray(const mavlink_camera_image_captured_t& imageCaptured);

signals:
    void dataReceived(const QByteArray &data);
    void locationDataRecieved(int sysid, float latitude, float longitude, int32_t altitude);
    void yawDataRecieved(int sysid, float yaw);
    void airspeedDataReceived(int id, float speed);
    void pressureDataReceived(int id, float pressure);
    void imageCapturedSignal(int width, int height, QByteArray imageData);

protected:
    void run() override;

private:
    int m_socket_fd;
};



class UDPManager : public QObject {
    Q_OBJECT
public:
    explicit UDPManager(MavLinkProperties *mavlinkProperties, QObject *parent = nullptr);
    ~UDPManager();

    int initialize(const QString& ipString, int port);
    void stop();


    sockaddr_in src_addr() const;
    void setSrc_addr(const sockaddr_in &newSrc_addr);
    int socket_fd() const;
    socklen_t src_addr_len() const;
    bool src_addr_set() const;
    void setSrc_addr_set(bool newSrc_addr_set);
    bool isConnected() const;
    const sockaddr* getSourceAddress() const;

signals:
    void connected();
    void disconnected();
    void errorOccurred(int errorCode);
    void dataReceived(const QByteArray& data);
    void locationDataRecieved(int sysid, float latitude, float longitude, int32_t altitude);
    void yawDataRecieved(int sysid, float yaw);
    void airspeedDataReceived(int id, float speed);
    void pressureDataReceived(int id, float pressure);
    void onConnected();
    void imageCapturedSignal(int width, int height, QByteArray imageData);
public slots:
    void onErrorOccurred(int errorCode);
    void onDisconnect();

    void onReadyRead();
private:
    struct sockaddr_in m_addr;
    struct sockaddr_in m_src_addr;
    int m_socket_fd;
    bool m_src_addr_set;
    socklen_t m_src_addr_len;
    UDPReceiverThread *m_receiverThread;
    QTimer *m_timer;
    MavLinkProperties *m_mavlinkProperties;
private slots:
    void handleReceivedData(const QByteArray &data);
};

#endif // UDPMANAGER_H
