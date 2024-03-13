#ifndef MAVLINKUDP_H
#define MAVLINKUDP_H

#include "planecontroller.h"

#include <QObject>
#include <QTimer>
#include <QThread>
#include <utils/httpclient.h>
#include <utils/mavlinkproperties.h>
#include <utils/teknofestproperties.h>
#include <utils/udpmanager.h>
#include <serial/serialmanager.h>

class MavLinkUDP : public QObject {
    Q_OBJECT
public:
    explicit MavLinkUDP(MavLinkProperties *mavlinkProperties, PlaneController *m_planeController, TeknofestProperties* teknofestProperties, HttpClient* httpClient, QObject *parent);

    Q_INVOKABLE int initialize(const QString& ipString, int port);
    Q_INVOKABLE int connectSerial(const QString& serialPort, int baudRate);

    UDPManager& getUDPManager() { return m_udpManager; }
    SerialManager& serialManager() { return m_serialManager; }
    MavLinkProperties* getMavLinkProperties() { return m_mavLinkProperties; }

    PlaneController* getPlaneController() {return m_planeController;}


    ~MavLinkUDP();
    int send_heartbeat();
    TeknofestProperties *teknofestProperties() const;
    Q_INVOKABLE plane* findMainPlane();
    plane *createDummyPlane();




    void initTeknofest();
signals:
    // Inherited signals from UDPManager


    // Inherited signals from MavLinkProperties
    void connectedChanged(bool connected);
    void heartbeatReceived();
    // New signals specific to MavLink
    void connected();

    void imageCapturedSignal(int width, int height, QByteArray imageData);



    //void dataReceived(const ssize_t &data);

    void errorOccurred(int errorCode);

    void HttpClientChanged();
    void receiveHeartbeat();
    void onArmedChangedSlot(bool armed);

public slots:
    void locationDataRecieved(int sysid, float latitude, float longitude, int32_t altitude);
    void yawDataRecieved(int sysid, float yaw);
    void airspeedDataReceived(int id, float speed);
    void pressureDataReceived(int id, float pressure);
    void batteryDataReceived(int sysid, float voltage, float current, float remainingCapacity);
    void armedChanged(bool armed, bool forced);
    void flyStateChanged(bool isFlying);
    void onDisconnect(int sysid);
    void flyingStateChanged(bool isFlying);


private:
    UDPManager m_udpManager;
    SerialManager m_serialManager;
    MavLinkProperties *m_mavLinkProperties;
    PlaneController *m_planeController;
    TeknofestProperties *m_teknofestProperties;
    HttpClient *m_httpClient;
    QTimer m_heartbeatTimer;
    void stopHeartbeat();

private slots:
    // Slot to handle armedChanged signal

    //void handleReceivedData(const ssize_t &data);
    void onHeartbeatTimeout();
    void sendHeartbeat();
};

#endif // MAVLINKUDP_H
