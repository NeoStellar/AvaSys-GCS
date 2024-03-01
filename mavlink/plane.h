#ifndef PLANE_H
#define PLANE_H
#include "qdatetime.h"
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
    Q_PROPERTY(bool isSelected READ isSelected WRITE setIsSelected NOTIFY isSelectedChanged FINAL)
    Q_PROPERTY(int teamid READ teamid WRITE setTeamid NOTIFY teamidChanged FINAL)
public:
    struct LocationData {
        QDateTime gpsSaati;
        int hedefGenislik;
        int hedefMerkezX;
        int hedefMerkezY;
        int hedefYukseklik;
        int ihaBatarya;
        double ihaBoylam;
        double ihaDikilme;
        double ihaEnlem;
        double ihaHiz;
        double ihaIrtifa;
        int ihaKilitlenme;
        int ihaOtonom;
        double ihaYatis;
        double ihaYonelme;
        int takimNumarasi;
    };

    // Getter and setter methods for location data
    Q_INVOKABLE const LocationData &locationData() const;
    void setLocationData(const LocationData& data);

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

    Q_INVOKABLE bool isSelected() const;
    void setIsSelected(bool newIsSelected);

    QJsonObject toJson() const;

    int teamid() const;
    void setTeamid(int newTeamid);

    int getUniqueIdentifier();

signals:

    void sysidChanged();
    void propertiesChanged();
    void longitudeChanged();

    void latitudeChanged();

    void altitudeChanged();

    void locationChanged(int sysid, float longitude, int latitude, int32_t altitude);

    void yawChanged();

    void isSelectedChanged();

    void teamidChanged();

private:
    int m_sysid;

    float m_longitude;
    float m_latitude;
    int32_t m_altitude;
    float m_yaw;
    bool m_isSelected;
    LocationData m_locationData;
    int m_teamid;
};

#endif // PLANE_H
