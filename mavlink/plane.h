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
    Q_PROPERTY(float roll READ roll WRITE setRoll NOTIFY rollChanged FINAL)
    Q_PROPERTY(float pitch READ pitch WRITE setPitch NOTIFY pitchChanged FINAL)
    Q_PROPERTY(int32_t altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged FINAL)
    Q_PROPERTY(float yaw READ yaw WRITE setYaw NOTIFY yawChanged FINAL)
    Q_PROPERTY(bool isSelected READ isSelected WRITE setIsSelected NOTIFY isSelectedChanged FINAL)
    Q_PROPERTY(int teamid READ teamid WRITE setTeamid NOTIFY teamidChanged FINAL)
    Q_PROPERTY(float airspeed READ airspeed WRITE setAirspeed NOTIFY airspeedChanged FINAL)
    Q_PROPERTY(float pressure READ pressure WRITE setPressure NOTIFY pressureChanged FINAL)
    //Q_PROPERTY(LocationData locationData READ locationData WRITE setLocationData NOTIFY locationDataChanged)
    Q_PROPERTY(QDateTime gpsSaati READ gpsSaati WRITE setGpsSaati NOTIFY gpsSaatiChanged FINAL)
    Q_PROPERTY(int hedefGenislik READ hedefGenislik WRITE setHedefGenislik NOTIFY hedefGenislikChanged FINAL)
    Q_PROPERTY(int hedefMerkezX READ hedefMerkezX WRITE setHedefMerkezX NOTIFY hedefMerkezXChanged FINAL)
    Q_PROPERTY(int hedefMerkezY READ hedefMerkezY WRITE setHedefMerkezY NOTIFY hedefMerkezYChanged FINAL)
    Q_PROPERTY(int hedefYukseklik READ hedefYukseklik WRITE setHedefYukseklik NOTIFY hedefYukseklikChanged FINAL)
    Q_PROPERTY(int ihaBatarya READ ihaBatarya WRITE setIhaBatarya NOTIFY ihaBataryaChanged FINAL)
    Q_PROPERTY(double ihaBoylam READ ihaBoylam WRITE setIhaBoylam NOTIFY ihaBoylamChanged FINAL)
    Q_PROPERTY(double ihaDikilme READ ihaDikilme WRITE setIhaDikilme NOTIFY ihaDikilmeChanged FINAL)
    Q_PROPERTY(double ihaEnlem READ ihaEnlem WRITE setIhaEnlem NOTIFY ihaEnlemChanged FINAL)
    Q_PROPERTY(double ihaHiz READ ihaHiz WRITE setIhaHiz NOTIFY ihaHizChanged FINAL)
    Q_PROPERTY(double ihaIrtifa READ ihaIrtifa WRITE setIhaIrtifa NOTIFY ihaIrtifaChanged FINAL)
    Q_PROPERTY(int ihaKilitlenme READ ihaKilitlenme WRITE setIhaKilitlenme NOTIFY ihaKilitlenmeChanged FINAL)
    Q_PROPERTY(int ihaOtonom READ ihaOtonom WRITE setIhaOtonom NOTIFY ihaOtonomChanged FINAL)
    Q_PROPERTY(double ihaYatis READ ihaYatis WRITE setIhaYatis NOTIFY ihaYatisChanged FINAL)
    Q_PROPERTY(double ihaYonelme READ ihaYonelme WRITE setIhaYonelme NOTIFY ihaYonelmeChanged FINAL)
    Q_PROPERTY(float voltage READ voltage WRITE setVoltage NOTIFY voltageChanged FINAL)
    Q_PROPERTY(float current READ current WRITE setCurrent NOTIFY currentChanged FINAL)
    Q_PROPERTY(float remainingCapacity READ remainingCapacity WRITE setRemainingCapacity NOTIFY remainingCapacityChanged FINAL)
public:

    /*
    struct LocationData {
        Q_GADGET
        Q_PROPERTY(double ihaBoylam MEMBER ihaBoylam)
    public:
        QDateTime gpsSaati;
        int hedefGenislik;
        int hedefMerkezX;
        int hedefMerkezY;
        int hedefYukseklik;
        int ihaBatarya;
        Q_INVOKABLE double ihaBoylam;
        double ihaDikilme;
        double ihaEnlem;
        double ihaHiz;
        double ihaIrtifa;
        int ihaKilitlenme;
        int ihaOtonom;
        double ihaYatis;
        double ihaYonelme;
        int takimNumarasi;
    }; */

    // Getter and setter methods for location data
    /*Q_INVOKABLE const LocationData locationData() const;
    void setLocationData(const LocationData& data);
    */
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

    Q_INVOKABLE int teamid() const;
    void setTeamid(int newTeamid);

    Q_INVOKABLE int getUniqueIdentifier() const;

    Q_INVOKABLE float airspeed() const;
    void setAirspeed(float newAirspeed);

    Q_INVOKABLE float pressure() const;
    void setPressure(float newPressure);

    Q_INVOKABLE QDateTime gpsSaati() const;
    void setGpsSaati(const QDateTime &newGpsSaati);

    Q_INVOKABLE int hedefGenislik() const;
    void setHedefGenislik(int newHedefGenislik);

    Q_INVOKABLE int hedefMerkezX() const;
    void setHedefMerkezX(int newHedefMerkezX);

    Q_INVOKABLE int hedefMerkezY() const;
    void setHedefMerkezY(int newHedefMerkezY);

    Q_INVOKABLE int hedefYukseklik() const;
    void setHedefYukseklik(int newHedefYukseklik);

    Q_INVOKABLE int ihaBatarya() const;
    void setIhaBatarya(int newIhaBatarya);

    Q_INVOKABLE double ihaBoylam() const;
    void setIhaBoylam(double newIhaBoylam);

    Q_INVOKABLE double ihaEnlem() const;
    void setIhaEnlem(double newIhaEnlem);

    Q_INVOKABLE double ihaHiz() const;
    void setIhaHiz(double newIhaHiz);

    Q_INVOKABLE double ihaIrtifa() const;
    void setIhaIrtifa(double newIhaIrtifa);

    Q_INVOKABLE int ihaKilitlenme() const;
    void setIhaKilitlenme(int newIhaKilitlenme);

    Q_INVOKABLE int ihaOtonom() const;
    void setIhaOtonom(int newIhaOtonom);

    Q_INVOKABLE double ihaYatis() const;
    void setIhaYatis(double newIhaYatis);

    Q_INVOKABLE double ihaYonelme() const;
    void setIhaYonelme(double newIhaYonelme);

    Q_INVOKABLE double ihaDikilme() const;
    void setIhaDikilme(double newIhaDikilme);

    float voltage() const;
    void setVoltage(float newVoltage);

    float current() const;
    void setCurrent(float newCurrent);

    float remainingCapacity() const;
    void setRemainingCapacity(float newRemainingCapacity);

    Q_INVOKABLE float roll() const;
    void setRoll(float newRoll);

    Q_INVOKABLE float pitch() const;
    void setPitch(float newPitch);

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

    void airspeedChanged();

    void pressureChanged();

    void locationDataChanged();

    void gpsSaatiChanged();

    void hedefGenislikChanged();

    void hedefMerkezXChanged();

    void hedefMerkezYChanged();

    void hedefYukseklikChanged();

    void ihaBataryaChanged();

    void ihaBoylamChanged();

    void ihaEnlemChanged();

    void ihaHizChanged();

    void ihaIrtifaChanged();

    void ihaKilitlenmeChanged();

    void ihaOtonomChanged();

    void ihaYatisChanged();

    Q_INVOKABLE void ihaYonelmeChanged();

    void ihaDikilmeChanged();

    void voltageChanged();

    void currentChanged();

    void remainingCapacityChanged();

    void rollChanged();

    void pitchChanged();

private:
    int m_sysid;

    float m_longitude;
    float m_latitude;
    int32_t m_altitude;
    float m_yaw;
    bool m_isSelected;
    //LocationData m_locationData;
    int m_teamid;
    float m_airspeed;
    float m_pressure;




    QDateTime m_gpsSaati;
    int m_hedefGenislik;
    int m_hedefMerkezX;
    int m_hedefMerkezY;
    int m_hedefYukseklik;
    int m_ihaBatarya;
    double m_ihaBoylam;
    double m_ihaEnlem;
    double m_ihaHiz;
    double m_ihaIrtifa;
    int m_ihaKilitlenme;
    int m_ihaOtonom;
    double m_ihaYatis;
    double m_ihaYonelme;
    double m_ihaDikilme;
    float m_voltage;
    float m_current;
    float m_remainingCapacity;
    float m_roll;
    float m_pitch;
};
#endif // PLANE_H
