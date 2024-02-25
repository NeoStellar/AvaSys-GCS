#include "plane.h"
#include "qdebug.h"
#include <cmath>

plane::plane(QObject *parent)
    : QObject{parent}
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
    emit locationChanged(m_sysid, m_longitude, m_latitude, m_altitude);
}





static const double PI = 3.14159265358979323846;

double degreeToRadian (const double degree) { return (degree * PI / 180); };
double radianToDegree (const double radian) { return (radian * 180 / PI); };
double CoordinatesToAngle (double latitude1,
                          const double longitude1,
                          double latitude2,
                          const double longitude2)
{
    const auto longitudeDifference = degreeToRadian(longitude2 - longitude1);
    latitude1 = degreeToRadian(latitude1);
    latitude2 = degreeToRadian(latitude2);

    using namespace std;
    const auto x = (cos(latitude1) * sin(latitude2)) -
                   (sin(latitude1) * cos(latitude2) * cos(longitudeDifference));
    const auto y = sin(longitudeDifference) * cos(latitude2);

    const auto degree = radianToDegree(atan2(y, x));
    return (degree >= 0)? degree : (degree + 360);
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
    //qDebug() << m_yaw;
    emit yawChanged(m_yaw);
    emit locationChanged(m_sysid, m_longitude, m_latitude, m_altitude);
}
