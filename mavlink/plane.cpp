#include "plane.h"
#include "qdebug.h"
#include <QJsonObject>
#include <QJsonDocument>

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

    //qDebug() << "long: " << m_longitude;

    // Emit signals to notify any connected slots
    //emit latitudeChanged();
    //emit longitudeChanged();
    //emit altitudeChanged();
    //qDebug() << m_sysid << ":" << m_yaw;
    //emit yawChanged();
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
    m_ihaYonelme = m_yaw;
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

    if(!m_gpsSaati.isNull()){
        locationDataObject["dakika"] = QDateTime::currentDateTime().time().minute();
        locationDataObject["milisaniye"] = QDateTime::currentDateTime().time().msec();
        locationDataObject["saat"] = QDateTime::currentDateTime().time().hour();
        locationDataObject["saniye"] = QDateTime::currentDateTime().time().second();
        locationJson["gps_saati"] = locationDataObject;

    }else {
        locationDataObject["dakika"] = m_gpsSaati.time().minute();
        locationDataObject["milisaniye"] = m_gpsSaati.time().msec();
        locationDataObject["saat"] = m_gpsSaati.time().hour();
        locationDataObject["saniye"] = m_gpsSaati.time().second();
        locationJson["gps_saati"] = locationDataObject;
    }

    locationJson["hedef_genislik"] = 10; //m_locationData.hedefGenislik;
    locationJson["hedef_merkez_X"] = 20; //m_locationData.hedefMerkezX;
    locationJson["hedef_merkez_Y"] = 920; //m_locationData.hedefMerkezY;
    locationJson["hedef_yukseklik"] = 31; //m_locationData.hedefYukseklik;
    locationJson["iha_batarya"] = 99; //m_locationData.ihaBatarya;
    locationJson["iha_boylam"] = m_longitude;

    locationJson["iha_dikilme"] = (int) m_pressure; //m_locationData.ihaDikilme;
    locationJson["iha_enlem"] = m_latitude;
    locationJson["iha_hiz"] = (int) m_airspeed; //m_locationData.ihaHiz;
    locationJson["iha_irtifa"] = (int) m_altitude;
    locationJson["iha_kilitlenme"] = 40; //m_locationData.ihaKilitlenme;
    locationJson["iha_otonom"] = 20; //m_locationData.ihaOtonom;
    locationJson["iha_yatis"] = 20; //m_locationData.ihaYatis;
    locationJson["iha_yonelme"] = m_yaw;
    //qDebug() << "a: " << m_teamid;
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

int plane::getUniqueIdentifier() const
{
    qDebug() << m_teamid;
    return m_teamid == -1 ? m_sysid : m_teamid;
}


float plane::airspeed() const
{
    return m_airspeed;
}

void plane::setAirspeed(float newAirspeed)
{
    if (qFuzzyCompare(m_airspeed, newAirspeed))
        return;
    m_airspeed = newAirspeed;
    emit airspeedChanged();
}

float plane::pressure() const
{
    return m_pressure;
}

void plane::setPressure(float newPressure)
{
    if (qFuzzyCompare(m_pressure, newPressure))
        return;
    m_pressure = newPressure;
    emit pressureChanged();
}

QDateTime plane::gpsSaati() const
{
    //qDebug() << "al eline";
    return m_gpsSaati;
}

void plane::setGpsSaati(const QDateTime &newGpsSaati)
{
    if (m_gpsSaati == newGpsSaati)
        return;
    m_gpsSaati = newGpsSaati;
    emit gpsSaatiChanged();
}

int plane::hedefGenislik() const
{
    return m_hedefGenislik;
}

void plane::setHedefGenislik(int newHedefGenislik)
{
    if (m_hedefGenislik == newHedefGenislik)
        return;
    m_hedefGenislik = newHedefGenislik;
    emit hedefGenislikChanged();
}

int plane::hedefMerkezX() const
{
    return m_hedefMerkezX;
}

void plane::setHedefMerkezX(int newHedefMerkezX)
{
    if (m_hedefMerkezX == newHedefMerkezX)
        return;
    m_hedefMerkezX = newHedefMerkezX;
    emit hedefMerkezXChanged();
}

int plane::hedefMerkezY() const
{
    return m_hedefMerkezY;
}

void plane::setHedefMerkezY(int newHedefMerkezY)
{
    if (m_hedefMerkezY == newHedefMerkezY)
        return;
    m_hedefMerkezY = newHedefMerkezY;
    emit hedefMerkezYChanged();
}

int plane::hedefYukseklik() const
{
    return m_hedefYukseklik;
}

void plane::setHedefYukseklik(int newHedefYukseklik)
{
    if (m_hedefYukseklik == newHedefYukseklik)
        return;
    m_hedefYukseklik = newHedefYukseklik;
    emit hedefYukseklikChanged();
}

int plane::ihaBatarya() const
{
    return m_ihaBatarya;
}

void plane::setIhaBatarya(int newIhaBatarya)
{
    if (m_ihaBatarya == newIhaBatarya)
        return;
    m_ihaBatarya = newIhaBatarya;
    emit ihaBataryaChanged();
}

double plane::ihaBoylam() const
{
    return m_ihaBoylam;
}

void plane::setIhaBoylam(double newIhaBoylam)
{
    if (qFuzzyCompare(m_ihaBoylam, newIhaBoylam))
        return;
    m_ihaBoylam = newIhaBoylam;
    emit ihaBoylamChanged();
}

double plane::ihaEnlem() const
{
    return m_ihaEnlem;
}

void plane::setIhaEnlem(double newIhaEnlem)
{
    if (qFuzzyCompare(m_ihaEnlem, newIhaEnlem))
        return;
    m_ihaEnlem = newIhaEnlem;
    emit ihaEnlemChanged();
}

double plane::ihaHiz() const
{
    return m_ihaHiz;
}

void plane::setIhaHiz(double newIhaHiz)
{
    if (qFuzzyCompare(m_ihaHiz, newIhaHiz))
        return;
    m_ihaHiz = newIhaHiz;
    emit ihaHizChanged();
}

double plane::ihaIrtifa() const
{
    return m_ihaIrtifa;
}

void plane::setIhaIrtifa(double newIhaIrtifa)
{
    if (qFuzzyCompare(m_ihaIrtifa, newIhaIrtifa))
        return;
    m_ihaIrtifa = newIhaIrtifa;
    emit ihaIrtifaChanged();
}

int plane::ihaKilitlenme() const
{
    return m_ihaKilitlenme;
}

void plane::setIhaKilitlenme(int newIhaKilitlenme)
{
    if (m_ihaKilitlenme == newIhaKilitlenme)
        return;
    m_ihaKilitlenme = newIhaKilitlenme;
    emit ihaKilitlenmeChanged();
}

int plane::ihaOtonom() const
{
    return m_ihaOtonom;
}

void plane::setIhaOtonom(int newIhaOtonom)
{
    if (m_ihaOtonom == newIhaOtonom)
        return;
    m_ihaOtonom = newIhaOtonom;
    emit ihaOtonomChanged();
}

double plane::ihaYatis() const
{
    return m_ihaYatis;
}

void plane::setIhaYatis(double newIhaYatis)
{
    if (qFuzzyCompare(m_ihaYatis, newIhaYatis))
        return;
    m_ihaYatis = newIhaYatis;
    emit ihaYatisChanged();
}

double plane::ihaYonelme() const
{
    return m_ihaYonelme;
}

void plane::setIhaYonelme(double newIhaYonelme)
{
    if (qFuzzyCompare(m_ihaYonelme, newIhaYonelme))
        return;
    m_ihaYonelme = newIhaYonelme;
    emit ihaYonelmeChanged();
}

double plane::ihaDikilme() const
{
    return m_ihaDikilme;
}

void plane::setIhaDikilme(double newIhaDikilme)
{
    if (qFuzzyCompare(m_ihaDikilme, newIhaDikilme))
        return;
    m_ihaDikilme = newIhaDikilme;
    emit ihaDikilmeChanged();
}

float plane::voltage() const
{
    return m_voltage;
}

void plane::setVoltage(float newVoltage)
{
    if (qFuzzyCompare(m_voltage, newVoltage))
        return;
    m_voltage = newVoltage;
    emit voltageChanged();
}

float plane::current() const
{
    return m_current;
}

void plane::setCurrent(float newCurrent)
{
    if (qFuzzyCompare(m_current, newCurrent))
        return;
    m_current = newCurrent;
    emit currentChanged();
}

float plane::remainingCapacity() const
{
    return m_remainingCapacity;
}

void plane::setRemainingCapacity(float newRemainingCapacity)
{
    if (qFuzzyCompare(m_remainingCapacity, newRemainingCapacity))
        return;
    m_remainingCapacity = newRemainingCapacity;
    emit remainingCapacityChanged();
}
