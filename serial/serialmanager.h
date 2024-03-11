#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include "mavlink-out/common/mavlink.h"
#include "utils/mavlinkproperties.h"
#include <QObject>
#include <QThread>
#include <QTimer>
#include <netinet/in.h>
#include <QtSerialPort/QSerialPort>



class SerialManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialManager(MavLinkProperties *mavlinkProperties, QObject *parent = nullptr);
    ~SerialManager();
    int initialize(const QString& serialPort, int baudrate);
    void stop();

    bool isConnected() const;

    QSerialPort *serialPort() const;
    void setSerialPort(QSerialPort *newSerialPort);
    void sendMAVLinkCommand(int sysid, uint16_t command, float param1, float param2, float param3, float param4, float param5, float param6, float param7);
    QByteArray convertImageToQByteArray(const mavlink_camera_image_captured_t& imageCaptured);
signals:
    void connected();
    void disconnected();
    void errorOccurred(QString error);
    void dataReceived(const QByteArray& data);
    void locationDataRecieved(int sysid, float latitude, float longitude, int32_t altitude);
    void yawDataRecieved(int sysid, float yaw);
    void airspeedDataReceived(int id, float speed);
    void pressureDataReceived(int id, float pressure);
    void onConnected();
    void imageCapturedSignal(int width, int height, QByteArray imageData);
    void batteryDataReceived(int sysid, float voltage, float current, float remainingCapacity);

    void handleReceivedData(const QByteArray &data);
    void onErrorOccurred(int errorCode);
    void onDisconnect();
    void onReadyRead();
public slots:

    void armDataReceived(int sysid, bool armed);

    void handle();

private:
    QSerialPort* m_serialPort;
    QTimer *m_timer;
    MavLinkProperties *m_mavlinkProperties;

};

#endif // SERIALMANAGER_H
