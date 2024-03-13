#include "mavlinkproperties.h"
#include "qdebug.h"

MavLinkProperties::MavLinkProperties(QObject *parent)
    : QObject(parent),
    m_armed(false),
    m_connected(false),
    m_isSerial(false),
    m_isFlying(false)
{
    qRegisterMetaType<int32_t>("int32_t");
    qRegisterMetaType<std::vector<std::string>>("std::vector<std::string>");
    qRegisterMetaType<std::string>("std::string");
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


// TODO need to clean up these
std::vector<std::string> MavLinkProperties::serialPorts() const
{
    qDebug() << m_serialPorts[0].c_str();
    return m_serialPorts;
}

QStringList MavLinkProperties::ports() const
{
    QStringList stringList;

    for(std::string str : m_serialPorts){
        stringList << QString::fromStdString(str);
    }

    return stringList;
}

void MavLinkProperties::setSerialPorts(const std::vector<std::string> &newSerialPorts)
{
    if (m_serialPorts == newSerialPorts)
        return;
    m_serialPorts = newSerialPorts;
    emit serialPortsChanged();
}

bool MavLinkProperties::isFlying() const
{
    return m_isFlying;
}

void MavLinkProperties::setIsFlying(bool newIsFlying)
{
    if (m_isFlying == newIsFlying)
        return;
    m_isFlying = newIsFlying;
    emit isFlyingChanged();
}
