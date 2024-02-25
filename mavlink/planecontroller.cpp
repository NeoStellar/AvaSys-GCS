#include "planecontroller.h"
#include "qdebug.h"
#include "QList"

PlaneController::PlaneController(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<int32_t>("int32_t");
    qRegisterMetaType<std::vector<plane*>>("std::vector<plane*>");
    //m_planes = QList<plane*>();
}
void PlaneController::setQmlContext(QQmlContext *context)
{
    context->setContextProperty("planeController",  this);
    context->setContextProperty("planeList", QVariant::fromValue(m_planes));
}

void PlaneController::addPlane(int sysid, double latitude, double longitude, int altitude)
{

    //plane *plane = new class plane();
    plane *plan = new plane();
    plan->setSysid(sysid);
    plan->setLatitude(latitude);
    plan->setLongitude(longitude);
    plan->setAltitude(altitude);
    if (plan != nullptr) {
        m_planes.push_back(plan);
    } else {
        qDebug() << "Error: Attempting to append a null plane object to m_planes.";
    }
    //connect(plane, &plane::locationChanged, this, &PlaneController::locationChanged);
}

void PlaneController::locationChanged(int sysid, float longitude, int latitude, int32_t altitude){
    addOrUpdatePlane(sysid, latitude, longitude, altitude);
}

std::vector<plane *> PlaneController::planes() const
{
    return m_planes;
}

void PlaneController::setPlanes(const std::vector<plane *> &newPlanes)
{
    if (m_planes == newPlanes)
        return;
    m_planes = newPlanes;
    emit planesChanged();
}

void PlaneController::removePlane(int sysid)
{
    for (int i = 0; i < m_planes.size(); ++i) {
        if (m_planes[i]->sysid() == sysid) {
            //delete m_planes.takeAt(i);
            return;
        }
    }
}

void PlaneController::updatePlane(int sysid, double latitude, double longitude, int altitude)
{
    for (plane *plane : m_planes) {
        if (plane->sysid() == sysid) {
            plane->updateLocation(latitude, longitude, altitude);
            return;
        }
    }
}

void PlaneController::addOrUpdatePlane(int sysid, double latitude, double longitude, int altitude)
{
    qDebug() << "a";

    for (int i = 0; i < m_planes.size(); ++i) {
        if (m_planes[i]->sysid() == sysid) {
            updatePlane(sysid, latitude, longitude, altitude);
        }
    }
    addPlane(sysid, latitude, longitude, altitude);
}

void PlaneController::updateYaw(int sysid, float yaw)
{
    for (plane *plane : m_planes) {
        if (plane->sysid() == sysid) {
            plane->setYaw(yaw);
            return;
        }
    }
}


