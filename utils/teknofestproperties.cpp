#include "teknofestproperties.h"

TeknofestProperties::TeknofestProperties(QObject *parent) : QObject{parent},
    m_session_id("asdas"),
    m_simMode(true)
{

}


int TeknofestProperties::takimid() const
{
    return m_takimid;
}

void TeknofestProperties::setTakimid(int newTakimid)
{
    if (m_takimid == newTakimid)
        return;
    m_takimid = newTakimid;
    emit takimidChanged();
}

QByteArray TeknofestProperties::session() const
{
    return m_session_id;
}

void TeknofestProperties::setsession(const QByteArray &newSession_id)
{
    if (m_session_id == newSession_id)
        return;
    m_session_id = newSession_id;
    emit sessionChanged();
}

bool TeknofestProperties::simMode() const
{
    return m_simMode;
}

void TeknofestProperties::setSimMode(bool newSimMode)
{
    if (m_simMode == newSimMode)
        return;
    m_simMode = newSimMode;
    emit simModeChanged();
}

std::vector<int> TeknofestProperties::planeids() const
{
    return m_planeids;
}

void TeknofestProperties::setPlaneids(const std::vector<int> &newPlaneids)
{
    if (m_planeids == newPlaneids)
        return;
    m_planeids = newPlaneids;
    emit planeidsChanged();
}
