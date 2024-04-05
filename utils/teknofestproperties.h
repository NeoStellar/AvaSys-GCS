#ifndef TEKNOFESTPROPERTIES_H
#define TEKNOFESTPROPERTIES_H


#include <QObject>
class TeknofestProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int takimid READ takimid WRITE setTakimid NOTIFY takimidChanged FINAL)
    Q_PROPERTY(QByteArray session_id READ session WRITE setsession NOTIFY sessionChanged FINAL)
    Q_PROPERTY(bool simMode READ simMode WRITE setSimMode NOTIFY simModeChanged FINAL)
    Q_PROPERTY(std::vector<int> planeids READ planeids WRITE setPlaneids NOTIFY planeidsChanged FINAL)
    Q_PROPERTY(double qrLatitude READ qrLatitude WRITE setQrLatitude NOTIFY qrLatitudeChanged FINAL)
    Q_PROPERTY(double qrLongitude READ qrLongitude WRITE setQrLongitude NOTIFY qrLongitudeChanged FINAL)
public:
    explicit TeknofestProperties(QObject *parent = nullptr);

    int takimid() const;
    void setTakimid(int newTakimid);

    QByteArray session() const;
    void setsession(const QByteArray &newSession_id);

    Q_INVOKABLE bool simMode() const;
    void setSimMode(bool newSimMode);

    std::vector<int> planeids() const;
    void setPlaneids(const std::vector<int> &newPlaneids);

    void addPlane(int id);

    double qrLatitude() const;
    void setQrLatitude(double newQrLatitude);

    double qrLongitude() const;
    void setQrLongitude(double newQrLongitude);

signals:
    void takimidChanged();

    void sessionChanged();

    void simModeChanged();

    void planeidsChanged();


    void QRLocationChanged();

    void qrLatitudeChanged();

    void qrLongitudeChanged();

private:
    int m_takimid;
    QByteArray m_session_id;
    bool m_simMode;
    std::vector<int> m_planeids;
    double m_qrLatitude;
    double m_qrLongitude;
};

#endif // TEKNOFESTPROPERTIES_H
