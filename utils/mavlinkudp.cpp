#include "mavlinkudp.h"

#include <QObject>

class mavlinkudp : public QObject {
    Q_OBJECT
public:
    mavlinkudp(QObject *parent = nullptr) : QObject(parent), is_connected(false), armed(false), connected(false) {}

    bool is_connected() const { return m_is_connected; }
    void setIs_connected(bool connected) {
        m_is_connected = connected;
        emit is_connectedChanged();
    }

    bool armed() const { return m_armed; }
    void setArmed(bool armed) {
        m_armed = armed;
        emit armedChanged();
    }

    bool connected() const { return m_connected; }
    void setConnected(bool connected) {
        m_connected = connected;
        emit connectedChanged();
    }

signals:
    void is_connectedChanged();
    void armedChanged();
    void connectedChanged();

private:
    bool m_is_connected;
    bool m_armed;
    bool m_connected;
};

