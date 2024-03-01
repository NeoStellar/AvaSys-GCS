#ifndef MAVLINKPROPERTIES_H
#define MAVLINKPROPERTIES_H

#include <QObject>

class MavLinkProperties : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool armed READ armed WRITE setArmed NOTIFY armedChanged FINAL)
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged FINAL)
    Q_PROPERTY(int sysid READ sysid WRITE setSysid NOTIFY sysidChanged FINAL)

public:
    explicit MavLinkProperties(QObject *parent = nullptr);

    bool armed() const;
    Q_INVOKABLE void setArmed(bool armed);

    Q_INVOKABLE bool connected() const;
    Q_INVOKABLE void setConnected(bool connected);

    Q_INVOKABLE void toggleArmStatus();

    Q_INVOKABLE int sysid() const;
    void setSysid(int newSysid);

signals:
    void armedChanged(bool armed);
    void connectedChanged(bool connected);

    void sysidChanged();

private:
    bool m_armed;
    bool m_connected;
    int m_sysid;
};

#endif // MAVLINKPROPERTIES_H
