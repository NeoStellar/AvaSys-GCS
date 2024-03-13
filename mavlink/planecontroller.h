// Controller class header file (e.g., PlaneController.h)
#pragma once

#include "plane.h"
#include <QQmlContext>
#include <QTimer>
#include <vector>

class PlaneController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::vector<plane*> planes READ planes WRITE setPlanes NOTIFY planesChanged FINAL)
    Q_PROPERTY(int selectedid READ selectedid WRITE setSelectedid NOTIFY selectedidChanged FINAL)

public:
    struct planestatus {
        plane* plane2;
        bool isValid = false;
        bool isLocal = false;
    }st;
    explicit PlaneController(QObject *parent = nullptr);
    void setQmlContext(QQmlContext *context);

    void setSelectedid(int newSelectedid);
    void setPlanes(const std::vector<plane *> &newPlanes);

    void removePlane(plane *plane);
    void removePlane(int sysid);
    struct planestatus quickCheck(int id);
    void updatePlane(plane *&plane, double latitude, double longitude, int altitude);

    Q_INVOKABLE int selectedid() const;
    Q_INVOKABLE plane *findMainPlane(int id);
    Q_INVOKABLE plane* findPlane(int id);
    Q_INVOKABLE plane* findSelected();
    Q_INVOKABLE std::vector<plane *> &planes();

public slots:
    void addPlane(int sysid, double latitude, double longitude, int altitude);
    void updatePlane(int sysid, double latitude, double longitude, int altitude);
    void addOrUpdatePlane(int sysid, double latitude, double longitude, int altitude, bool simMode);

    // attributes
    void updateYaw(int sysid, float yaw);
    void updateSpeed(int id, float speed);
    void updatePressure(int sysid, float pressure);
    void updateBattery(int sysid, float voltage, float current, float remainingCapacity);

    void setTeam(int sysid, int teamid);
    Q_INVOKABLE void changeSelection(int systemid);
    void removeLocalPlane(int sysid);
signals:
    void planesChanged();
    void selectedidChanged();
private:
    std::vector<plane*> m_planes;
    int m_selectedid;
    QTimer m_updateTimer;
private slots:
    void throttledUpdate();
};
