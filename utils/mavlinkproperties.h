#ifndef MAVLINKPROPERTIES_H
#define MAVLINKPROPERTIES_H

#include <QObject>

class MavLinkProperties : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool armed READ armed WRITE setArmed NOTIFY armedChanged FINAL)
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged FINAL)
    Q_PROPERTY(int sysid READ sysid WRITE setSysid NOTIFY sysidChanged FINAL)
    Q_PROPERTY(bool isSerial READ isSerial WRITE setIsSerial NOTIFY isSerialChanged FINAL)
    Q_PROPERTY(bool isFlying READ isFlying WRITE setIsFlying NOTIFY isFlyingChanged FINAL)
    Q_PROPERTY(std::vector<std::string> serialPorts READ serialPorts WRITE setSerialPorts NOTIFY serialPortsChanged FINAL)

public:
    explicit MavLinkProperties(QObject *parent = nullptr);

    bool armed() const;
    Q_INVOKABLE void setArmed(bool armed);

    Q_INVOKABLE bool connected() const;
    Q_INVOKABLE void setConnected(bool connected);

    Q_INVOKABLE void toggleArmStatus(bool forced);

    Q_INVOKABLE int sysid() const;
    void setSysid(int newSysid);

    bool isSerial() const;
    void setIsSerial(bool newIsSerial);

    Q_INVOKABLE std::vector<std::string> serialPorts() const;
    Q_INVOKABLE QStringList ports() const;
    void setSerialPorts(const std::vector<std::string> &newSerialPorts);

    Q_INVOKABLE bool isFlying() const;
    Q_INVOKABLE void setIsFlying(bool newIsFlying);

signals:
    void armedChanged(bool armed);
    void armedStatusChanged(bool armed, bool forced);
    void connectedChanged(bool connected);

    void sysidChanged();

    void isSerialChanged();


    void serialPortsChanged();

    void isFlyingChanged();

private:
    bool m_armed;
    bool m_connected;
    int m_sysid;
    bool m_isSerial;
    std::vector<std::string> m_serialPorts;
    bool m_isFlying;
};

#endif // MAVLINKPROPERTIES_H
