#ifndef PLANE_H
#define PLANE_H
#include <QObject>

#include <utils/mavlinkproperties.h>

class plane : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int sysid READ sysid WRITE setSysid NOTIFY sysidChanged FINAL)
    Q_PROPERTY(float longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged FINAL)
    Q_PROPERTY(float latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged FINAL)
    Q_PROPERTY(int32_t altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged FINAL)
    Q_PROPERTY(float yaw READ yaw WRITE setYaw NOTIFY yawChanged FINAL)
public:
    explicit plane(QObject *parent = nullptr);

    Q_INVOKABLE int sysid() const;
    void setSysid(int newSysid);

    Q_INVOKABLE float longitude() const;
    void setLongitude(float newLongitude);

    Q_INVOKABLE float latitude() const;
    void setLatitude(float newLatitude);

    Q_INVOKABLE int32_t altitude() const;
    void setAltitude(int32_t newAltitude);

    void updateLocation(double newLatitude, double newLongitude, int newAltitude);

    Q_INVOKABLE float yaw() const;
    void setYaw(float newYaw);

signals:

    void sysidChanged();
    void propertiesChanged();
    void longitudeChanged();

    void latitudeChanged();

    void altitudeChanged();

    void locationChanged(int sysid, float longitude, int latitude, int32_t altitude);

    void yawChanged(float yaw);

private:
    int m_sysid;

    float m_longitude;
    float m_latitude;
    int32_t m_altitude;
    float m_yaw;
};

#endif // PLANE_H
