#include "plane.h"
#include "qdebug.h"
#include <cmath>
#include <QJsonObject>
#include <QJsonDocument>
const plane::LocationData &plane::locationData() const
{
    return m_locationData;
}

void plane::setLocationData(const LocationData &data)
{
    m_locationData = data;
}

plane::plane(QObject *parent)
    : QObject{parent}, m_isSelected(false)
{
    qRegisterMetaType<int32_t>("int32_t");
}

int plane::sysid() const
{
    return m_sysid;
}

void plane::setSysid(int newSysid)
{
    if (m_sysid == newSysid)
        return;
    m_sysid = newSysid;
    emit sysidChanged();
}




float plane::longitude() const
{
    return m_longitude;
}

void plane::setLongitude(float newLongitude)
{
    if (qFuzzyCompare(m_longitude, newLongitude))
        return;
    m_longitude = newLongitude;

    emit longitudeChanged();
}

float plane::latitude() const
{
    return m_latitude;
}

void plane::setLatitude(float newLatitude)
{
    if (qFuzzyCompare(m_latitude, newLatitude))
        return;
    m_latitude = newLatitude;

    emit latitudeChanged();
}

int32_t plane::altitude() const
{
    return m_altitude;
}

void plane::setAltitude(int32_t newAltitude)
{
    if (m_altitude == newAltitude)
        return;
    m_altitude = newAltitude;
    emit altitudeChanged();
}

void plane::updateLocation(double newLatitude, double newLongitude, int newAltitude)
{
    /* if (qFuzzyCompare(m_latitude, newLatitude) &&
        qFuzzyCompare(m_longitude, newLongitude) &&
        m_altitude == newAltitude)
    {
        // No change in location, return
        return;
    } */
    m_latitude = newLatitude;
    m_longitude = newLongitude;
    m_altitude = newAltitude;

    // Emit signals to notify any connected slots
    emit latitudeChanged();
    emit longitudeChanged();
    emit altitudeChanged();
    //qDebug() << m_sysid << ":" << m_yaw;
    emit yawChanged();
    if(m_sysid == 3){
        //qDebug() << m_sysid << ":" << m_isSelected;
    }
    emit locationChanged(m_sysid, m_longitude, m_latitude, m_altitude);

}

float plane::yaw() const
{
    return m_yaw;
}

void plane::setYaw(float newYaw)
{
    if (qFuzzyCompare(m_yaw, newYaw))
        return;
    m_yaw = newYaw;
    m_locationData.ihaYonelme = m_yaw;
    //qDebug() << m_sysid << ":" << m_yaw;
    emit yawChanged();
    //emit locationChanged(m_sysid, m_longitude, m_latitude, m_altitude);
}

bool plane::isSelected() const
{
    return m_isSelected;
}

void plane::setIsSelected(bool newIsSelected)
{
    if (m_isSelected == newIsSelected)
        return;
    m_isSelected = newIsSelected;
    emit isSelectedChanged();
}

QJsonObject plane::toJson() const{
    QJsonObject json;

    // Convert location data to JSON
    QJsonObject locationJson;
    //m_locationData.gpsSaati = QDateTime::currentDateTime();
    QJsonObject locationDataObject;
    /*
    if(!m_locationData.gpsSaati.isNull()){
        locationDataObject["dakika"] = QDateTime::currentDateTime().time().minute();
        locationDataObject["milisaniye"] = QDateTime::currentDateTime().time().msec();
        locationDataObject["saat"] = QDateTime::currentDateTime().time().hour();
        locationDataObject["saniye"] = QDateTime::currentDateTime().time().second();
        locationJson["gps_saati"] = locationDataObject;

    }else {
        locationDataObject["dakika"] = m_locationData.gpsSaati.time().minute();
        locationDataObject["milisaniye"] = m_locationData.gpsSaati.time().msec();
        locationDataObject["saat"] = m_locationData.gpsSaati.time().hour();
        locationDataObject["saniye"] = m_locationData.gpsSaati.time().second();
        locationJson["gps_saati"] = locationDataObject;
    }*/

    locationJson["hedef_genislik"] = 10; //m_locationData.hedefGenislik;
    locationJson["hedef_merkez_X"] = 20; //m_locationData.hedefMerkezX;
    locationJson["hedef_merkez_Y"] = 920; //m_locationData.hedefMerkezY;
    locationJson["hedef_yukseklik"] = 31; //m_locationData.hedefYukseklik;
    locationJson["iha_batarya"] = 99; //m_locationData.ihaBatarya;
    locationJson["iha_boylam"] = m_longitude;
    locationJson["iha_dikilme"] = 50; //m_locationData.ihaDikilme;
    locationJson["iha_enlem"] = m_latitude;
    locationJson["iha_hiz"] = 30; //m_locationData.ihaHiz;
    locationJson["iha_irtifa"] = m_altitude;
    locationJson["iha_kilitlenme"] = 40; //m_locationData.ihaKilitlenme;
    locationJson["iha_otonom"] = 20; //m_locationData.ihaOtonom;
    locationJson["iha_yatis"] = 20; //m_locationData.ihaYatis;
    locationJson["iha_yonelme"] = m_yaw;
    qDebug() << "a: " << m_teamid;
    locationJson["takim_numarasi"] = m_teamid;

    json = locationJson;

    return json;
}

int plane::teamid() const
{
    return m_teamid;
}

void plane::setTeamid(int newTeamid)
{
    if (m_teamid == newTeamid)
        return;
    m_teamid = newTeamid;
    emit teamidChanged();
}

int plane::getUniqueIdentifier()
{
    return m_teamid == -1 ? m_sysid : m_teamid;
}
