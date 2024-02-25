// Controller class header file (e.g., PlaneController.h)
#pragma once

#include "plane.h"
#include <QQmlContext>
#include <vector>

class PlaneController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::vector<plane*> planes READ planes WRITE setPlanes NOTIFY planesChanged FINAL)

public:
    explicit PlaneController(QObject *parent = nullptr);
    void setQmlContext(QQmlContext *context);

    Q_INVOKABLE std::vector<plane *> planes() const;
    void setPlanes(const std::vector<plane *> &newPlanes);

public slots:
    void addPlane(int sysid, double latitude, double longitude, int altitude);
    void removePlane(int sysid);
    void updatePlane(int sysid, double latitude, double longitude, int altitude);
    void addOrUpdatePlane(int sysid, double latitude, double longitude, int altitude);

    void updateYaw(int sysid, float yaw);

signals:
    void locationChanged(int sysid, float longitude, int latitude, int32_t altitude);

    void planesChanged();

private:
    std::vector<plane*> m_planes;

};
