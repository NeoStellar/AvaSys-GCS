#include "teknocircles.h"

TeknoCircles::TeknoCircles(QObject *parent)
    : QObject(parent)
{
}

double TeknoCircles::latitude() const
{
    return m_latitude;
}

void TeknoCircles::setLatitude(double newLatitude)
{
    if (qFuzzyCompare(m_latitude, newLatitude))
        return;
    m_latitude = newLatitude;
    emit latitudeChanged();
}

double TeknoCircles::longitude() const
{
    return m_longitude;
}

void TeknoCircles::setLongitude(double newLongitude)
{
    if (qFuzzyCompare(m_longitude, newLongitude))
        return;
    m_longitude = newLongitude;
    emit longitudeChanged();
}

double TeknoCircles::altitude() const
{
    return m_altitude;
}

void TeknoCircles::setAltitude(double newAltitude)
{
    if (qFuzzyCompare(m_altitude, newAltitude))
        return;
    m_altitude = newAltitude;
    emit altitudeChanged();
}

double TeknoCircles::radius() const
{
    return m_radius;
}

void TeknoCircles::setRadius(double newRadius)
{
    if (qFuzzyCompare(m_radius, newRadius))
        return;
    m_radius = newRadius;
    emit radiusChanged();
}

QString TeknoCircles::name() const
{
    return m_name;
}

void TeknoCircles::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

int TeknoCircles::id() const
{
    return m_id;
}

void TeknoCircles::setId(int newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

TeknoCircles::TeknoCircles(double latitude, double longitude, double radius, const QString &name, int id, QObject *parent)
    : QObject(parent),
    m_latitude(latitude),
    m_longitude(longitude),
    m_radius(radius),
    m_name(name),
    m_id(id),
    m_opacity(1.0)
{
    // Emit signals for the properties initialized in the constructor
    emit latitudeChanged();
    emit longitudeChanged();
    emit altitudeChanged();
    emit radiusChanged();
    emit nameChanged();
    emit idChanged();
}

bool TeknoCircles::empty() const
{
    return m_empty;
}

void TeknoCircles::setEmpty(bool newEmpty)
{
    if (m_empty == newEmpty)
        return;
    m_empty = newEmpty;
    emit emptyChanged();
}

bool TeknoCircles::hasBorder() const
{
    return m_hasBorder;
}

void TeknoCircles::setHasBorder(bool newHasBorder)
{
    if (m_hasBorder == newHasBorder)
        return;
    m_hasBorder = newHasBorder;
    emit hasBorderChanged();
}

double TeknoCircles::borderSize() const
{
    return m_borderSize;
}

void TeknoCircles::setBorderSize(double newBorderSize)
{
    if (qFuzzyCompare(m_borderSize, newBorderSize))
        return;
    m_borderSize = newBorderSize;
    emit borderSizeChanged();
}

QString TeknoCircles::insideColor() const
{
    return m_insideColor;
}

void TeknoCircles::setInsideColor(const QString &newInsideColor)
{
    if (m_insideColor == newInsideColor)
        return;
    m_insideColor = newInsideColor;
    emit insideColorChanged();
}

QString TeknoCircles::borderColor() const
{
    return m_borderColor;
}

void TeknoCircles::setBorderColor(const QString &newBorderColor)
{
    if (m_borderColor == newBorderColor)
        return;
    m_borderColor = newBorderColor;
    setHasBorder(true);
    emit borderColorChanged();
}

double TeknoCircles::opacity() const
{
    return m_opacity;
}

void TeknoCircles::setOpacity(double newOpacity)
{
    if (qFuzzyCompare(m_opacity, newOpacity))
        return;
    m_opacity = newOpacity;
    emit opacityChanged();
}
