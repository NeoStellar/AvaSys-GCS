#include "mavlinkproperties.h"
#include "qdebug.h"

MavLinkProperties::MavLinkProperties(QObject *parent)
    : QObject(parent),
    m_armed(false),
    m_connected(false) {
}

bool MavLinkProperties::armed() const {
    return m_armed;
}

void MavLinkProperties::setArmed(bool armed) {
    qDebug() << "Input: " << armed;
    qDebug() << "Old variable: " << m_armed;
    m_armed = armed;
    qDebug() << "Newer variable: " << m_armed;
    emit armedChanged(armed);
}


bool MavLinkProperties::connected() const { return m_connected; }

void MavLinkProperties::setConnected(bool connected) {
    m_connected = connected;
    emit connectedChanged(connected);
}

void MavLinkProperties::toggleArmStatus()
{
    m_armed = !m_armed;
    emit armedChanged(m_armed);
}


