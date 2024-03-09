#include "mavlinkproperties.h"
#include "qdebug.h"

MavLinkProperties::MavLinkProperties(QObject *parent)
    : QObject(parent),
    m_armed(false),
    m_connected(false),
    m_isSerial(false)
{
    qRegisterMetaType<int32_t>("int32_t");
    qRegisterMetaType<std::vector<std::string>>("std::vector<std::string>");
}

bool MavLinkProperties::armed() const {
    return m_armed;
}

void MavLinkProperties::setArmed(bool armed) {
    //qDebug() << "Input: " << armed;
    //qDebug() << "Old variable: " << m_armed;
    m_armed = armed;
    //qDebug() << "Newer variable: " << m_armed;
    emit armedChanged(armed);
}


bool MavLinkProperties::connected() const { return m_connected; }

void MavLinkProperties::setConnected(bool connected) {
    m_connected = connected;
    emit connectedChanged(connected);
}

void MavLinkProperties::toggleArmStatus(bool forced)
{
    //m_armed = !m_armed;
    //emit armedChanged(m_armed);
    emit armedStatusChanged(!m_armed, forced);
}



int MavLinkProperties::sysid() const
{
    return m_sysid;
}

void MavLinkProperties::setSysid(int newSysid)
{
    if (m_sysid == newSysid)
        return;
    m_sysid = newSysid;
    emit sysidChanged();
}

bool MavLinkProperties::isSerial() const
{
    return m_isSerial;
}

void MavLinkProperties::setIsSerial(bool newIsSerial)
{
    if (m_isSerial == newIsSerial)
        return;
    m_isSerial = newIsSerial;
    emit isSerialChanged();
}

std::vector<std::string> MavLinkProperties::serialPorts() const
{
    return m_serialPorts;
}

void MavLinkProperties::setSerialPorts(const std::vector<std::string> &newSerialPorts)
{
    if (m_serialPorts == newSerialPorts)
        return;
    m_serialPorts = newSerialPorts;
    emit serialPortsChanged();
}
