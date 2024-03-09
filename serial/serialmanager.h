#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include "mavlink-out/common/mavlink.h"
#include "utils/mavlinkproperties.h"

#include <QObject>
#include <QThread>
#include <QTimer>
#include <netinet/in.h>
#include <QtSerialPort/QSerialPort>

class SerialReceiverThread : public QThread {
    Q_OBJECT

public:
    explicit SerialReceiverThread(QSerialPort* serialPort, QObject *parent = nullptr);
    ~SerialReceiverThread();
    QByteArray convertImageToQByteArray(const mavlink_camera_image_captured_t& imageCaptured);
    void setSerialPort(const QSerialPort &newSerialPort);

    QSerialPort *serialPort() {return m_serialPort;}

signals:
    void dataReceived(const QByteArray &data);
    void locationDataRecieved(int sysid, float latitude, float longitude, int32_t altitude);
    void yawDataRecieved(int sysid, float yaw);
    void airspeedDataReceived(int id, float speed);
    void pressureDataReceived(int id, float pressure);
    void imageCapturedSignal(int width, int height, QByteArray imageData);
    void batteryDataReceived(int sysid, float voltage, float current, float remainingCapacity);
    void armDataReceived(int sysid, bool armed);
protected:
    void run() override;
private:
    QSerialPort* m_serialPort;
};

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
    void armDataReceived(int sysid, bool armed);
    void handleReceivedData(const QByteArray &data);
public slots:
    void onErrorOccurred(int errorCode);
    void onDisconnect();

    void onReadyRead();
private:
    QSerialPort* m_serialPort;
    QTimer *m_timer;
    MavLinkProperties *m_mavlinkProperties;
    SerialReceiverThread *m_receiverThread;
};

#endif // SERIALMANAGER_H
