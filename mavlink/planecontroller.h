// Controller class header file (e.g., PlaneController.h)
#pragma once

#include "plane.h"
#include <QQmlContext>
#include <vector>

class PlaneController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::vector<plane*> planes READ planes WRITE setPlanes NOTIFY planesChanged FINAL)
    Q_PROPERTY(int selectedid READ selectedid WRITE setSelectedid NOTIFY selectedidChanged FINAL)

public:
    explicit PlaneController(QObject *parent = nullptr);
    void setQmlContext(QQmlContext *context);

    Q_INVOKABLE std::vector<plane *> &planes();
    void setPlanes(const std::vector<plane *> &newPlanes);

    Q_INVOKABLE plane* findSelected();

    Q_INVOKABLE plane* findPlane(int id);

    void removePlane(plane *plane);
    Q_INVOKABLE int selectedid() const;
    void setSelectedid(int newSelectedid);


    Q_INVOKABLE plane *findMainPlane(int id);
    void removePlane(int sysid);
public slots:
    void addPlane(int sysid, double latitude, double longitude, int altitude);

    void updatePlane(int sysid, double latitude, double longitude, int altitude);
    void addOrUpdatePlane(int sysid, double latitude, double longitude, int altitude);
    void AoULocalPlane(int sysid, double latitude, double longitude, int altitude);
    void AoUTeknoPlane(int teamid, double latitude, double longitude, int altitude);

    void addLocalPlane(int sysid, double latitude, double longitude, int altitude);
    void addTeknoPlane(int teamid, double latitude, double longitude, int altitude);
    void updateYaw(int sysid, float yaw);

    void updateLocalPlane(int sysid, double latitude, double longitude, int altitude);
    void updateTeknoPlane(int team, double latitude, double longitude, int altitude);

    void updateLocalYaw(int sysid, float yaw);
    void updateTeknoYaw(int teamid, float yaw);

    void updateLocalSpeed(int sysid, float speed);

    void updateLocalPressure(int sysid, float pressure);

    void updateLocalBattery(int sysid, float voltage, float current, float remainingCapacity);

    Q_INVOKABLE void changeLocalSelection(int sysid);
    Q_INVOKABLE void changeTeknoSelection(int teamid);

    void setTeam(int sysid, int teamid);
    Q_INVOKABLE void changeSelection(int systemid, int teamid);

    void removeLocalPlane(int sysid);

    void locationChanged(int sysid, float longitude, int latitude, int32_t altitude);

signals:

    void planesChanged();

    void selectedidChanged();

private:
    std::vector<plane*> m_planes;
    std::vector<plane*> *ptr;
    int m_selectedid;
};
