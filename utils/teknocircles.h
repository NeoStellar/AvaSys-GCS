#ifndef TEKNOCIRCLES_H
#define TEKNOCIRCLES_H

#include <QObject>

class TeknoCircles : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged FINAL)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged FINAL)
    Q_PROPERTY(double altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged FINAL)
    Q_PROPERTY(double radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY opacityChanged FINAL)
    Q_PROPERTY(bool empty READ empty WRITE setEmpty NOTIFY emptyChanged FINAL)
    Q_PROPERTY(bool hasBorder READ hasBorder WRITE setHasBorder NOTIFY hasBorderChanged FINAL)
    Q_PROPERTY(double borderSize READ borderSize WRITE setBorderSize NOTIFY borderSizeChanged FINAL)
    Q_PROPERTY(QString insideColor READ insideColor WRITE setInsideColor NOTIFY insideColorChanged FINAL)
    Q_PROPERTY(QString borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged FINAL)
public:
    explicit TeknoCircles(QObject *parent = nullptr);
    explicit TeknoCircles(double latitude, double longitude, double radius, const QString &name, int id, QObject *parent = nullptr);

    Q_INVOKABLE double latitude() const;
    void setLatitude(double newLatitude);

    Q_INVOKABLE double longitude() const;
    void setLongitude(double newLongitude);

    Q_INVOKABLE double altitude() const;
    void setAltitude(double newAltitude);

    Q_INVOKABLE double radius() const;
    void setRadius(double newRadius);

    Q_INVOKABLE QString name() const;
    void setName(const QString &newName);

    Q_INVOKABLE int id() const;
    void setId(int newId);

    Q_INVOKABLE bool empty() const;
    void setEmpty(bool newEmpty);

    Q_INVOKABLE bool hasBorder() const;
    void setHasBorder(bool newHasBorder);

    Q_INVOKABLE double borderSize() const;
    void setBorderSize(double newBorderSize);

    Q_INVOKABLE QString insideColor() const;
    void setInsideColor(const QString &newInsideColor);

    Q_INVOKABLE QString borderColor() const;
    void setBorderColor(const QString &newBorderColor);

    Q_INVOKABLE double opacity() const;
    void setOpacity(double newOpacity);

signals:
    void latitudeChanged();
    void longitudeChanged();
    void altitudeChanged();
    void radiusChanged();
    void nameChanged();
    void idChanged();

    void emptyChanged();

    void hasBorderChanged();

    void borderSizeChanged();

    void insideColorChanged();

    void borderColorChanged();

    void opacityChanged();

private:
    double m_latitude;
    double m_longitude;
    double m_altitude;
    double m_radius;
    QString m_name;
    int m_id;
    bool m_empty;
    bool m_hasBorder;
    double m_borderSize;
    QString m_insideColor;
    QString m_borderColor;
    double m_opacity;
};

#endif // TEKNOCIRCLES_H
