#ifndef MAVLINKUDP_H
#define MAVLINKUDP_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <utils/mavlinkproperties.h>
#include <utils/udpmanager.h>

class MavLinkUDP : public QObject {
    Q_OBJECT
public:
    explicit MavLinkUDP(MavLinkProperties *mavlinkProperties, QObject *parent);

    Q_INVOKABLE int initialize(const QString& ipString, int port);

    UDPManager& getUDPManager() { return m_udpManager; }
    MavLinkProperties* getMavLinkProperties() { return m_mavLinkProperties; }

    ~MavLinkUDP();
    int send_heartbeat();
signals:
    // Inherited signals from UDPManager


    // Inherited signals from MavLinkProperties
    void connectedChanged(bool connected);
    void heartbeatReceived();
    // New signals specific to MavLink
    void connected();



    void armedChanged(bool armed);
    void errorOccurred(int errorCode);
    void dataReceived(const QByteArray &data);



public slots:

    void receiveHeartbeat();
private:
    UDPManager m_udpManager;
    MavLinkProperties *m_mavLinkProperties;
    QTimer m_heartbeatTimer;
    void stopHeartbeat();
private slots:
    // Slot to handle armedChanged signal
    void onArmedChangedSlot(bool armed);
    void handleReceivedData(const QByteArray &data);
    void onHeartbeatTimeout();
    void sendHeartbeat();
};

#endif // MAVLINKUDP_H
