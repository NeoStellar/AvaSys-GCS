#ifndef MAVLINKUDP_H
#define MAVLINKUDP_H

#include "planecontroller.h"

#include <QObject>
#include <QTimer>
#include <QThread>
#include <utils/mavlinkproperties.h>
#include <utils/udpmanager.h>

class MavLinkUDP : public QObject {
    Q_OBJECT
public:
    explicit MavLinkUDP(MavLinkProperties *mavlinkProperties, PlaneController *m_planeController, QObject *parent);

    Q_INVOKABLE int initialize(const QString& ipString, int port);

    UDPManager& getUDPManager() { return m_udpManager; }
    MavLinkProperties* getMavLinkProperties() { return m_mavLinkProperties; }

    PlaneController* getPlaneController() {return m_planeController;}


    ~MavLinkUDP();
    int send_heartbeat();
signals:
    // Inherited signals from UDPManager


    // Inherited signals from MavLinkProperties
    void connectedChanged(bool connected);
    void heartbeatReceived();
    // New signals specific to MavLink
    void connected();


    void locationDataRecieved(float latitude, float longitude, int32_t altitude);
    void yawDataRecieved(float yaw);
    void armedChanged(bool armed);
    void errorOccurred(int errorCode);
    //void dataReceived(const ssize_t &data);



public slots:

    void receiveHeartbeat();
private:
    UDPManager m_udpManager;
    MavLinkProperties *m_mavLinkProperties;
    PlaneController *m_planeController;
    QTimer m_heartbeatTimer;
    void stopHeartbeat();
private slots:
    // Slot to handle armedChanged signal
    void onArmedChangedSlot(bool armed);
    //void handleReceivedData(const ssize_t &data);
    void onHeartbeatTimeout();
    void sendHeartbeat();
};

#endif // MAVLINKUDP_H
