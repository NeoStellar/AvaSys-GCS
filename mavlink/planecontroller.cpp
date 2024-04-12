#include "planecontroller.h"
#include "qdebug.h"
#include "QList"
#include "plane.h"
#include "plane.h"

PlaneController::PlaneController(NotificationCenter *notificationCenter, QObject *parent)
    : QObject(parent), m_notificationCenter(notificationCenter), m_selectedid(-5)
{
    qRegisterMetaType<int32_t>("int32_t");
    qRegisterMetaType<std::vector<plane*>>("std::vector<plane*>");
    m_updateTimer.setInterval(200);
    connect(&m_updateTimer, &QTimer::timeout, this, &PlaneController::throttledUpdate);
    m_updateTimer.start();
}
void PlaneController::setQmlContext(QQmlContext *context)
{
    context->setContextProperty("planeController",  this);
    context->setContextProperty("planeList", QVariant::fromValue(m_planes));
}

void PlaneController::addPlane(int teamid, double latitude, double longitude, int altitude)
{
    plane *plan = new plane();
    plan->setTeamid(teamid);
    plan->setLatitude(latitude);
    plan->setLongitude(longitude);
    plan->setAltitude(altitude);
    if (plan != nullptr) {
        m_planes.push_back(plan);
    } else {
        qDebug() << "Error: Attempting to append a null plane object to m_planes.";
    }
}

std::vector<plane *> &PlaneController::planes()
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

plane *PlaneController::findSelected()
{
    for(plane* plane : m_planes){
        if(plane->isSelected()){
            return plane;
        }
    }
    return nullptr;
}

plane *PlaneController::findPlane(int id)
{

    struct planestatus status = quickCheck(id);
    if(status.isValid){
        if(status.isLocal){
            qDebug() << "plane: " << status.plane2->sysid();
        }else {
            qDebug() << "plane team: " << status.plane2->teamid();
        }
        return status.plane2;
    }
    return nullptr;
}

void PlaneController::removePlane(plane* plane)
{
    m_planes.erase(std::remove(m_planes.begin(), m_planes.end(), plane), m_planes.end());
}

void PlaneController::updatePlane(int id, double latitude, double longitude, int altitude)
{
    struct planestatus check = quickCheck(id);
    if(check.isValid){
        check.plane2->setGpsSaati(QDateTime::currentDateTime());
        check.plane2->updateLocation(latitude, longitude, altitude);
        //emit planesChanged();
    }
}

void PlaneController::updatePlane(plane* &plane, double latitude, double longitude, int altitude){
    plane->updateLocation(latitude, longitude, altitude);
}

PlaneController::planestatus PlaneController::quickCheck(int id)
{
    struct planestatus r = st;
    for (plane *plane : m_planes) {
        plane->setGpsSaati(QDateTime::currentDateTime());
        if(plane->sysid() != -1){
            if(plane->sysid() == id){
                r.isValid = true;
                r.isLocal = true;
                r.plane2 = plane;
                break;
            }
        }else if (plane->teamid() != -1){
            if(plane->teamid() == id){
                r.isValid = true;
                r.isLocal = false;
                r.plane2 = plane;
                break;
            }
        }
    }
    return r;
}


void PlaneController::addOrUpdatePlane(int id, double latitude, double longitude, int altitude, bool simMode)
{
    struct planestatus a = quickCheck(id);

    if(a.isValid){
        a.plane2->setGpsSaati(QDateTime::currentDateTime());
        a.plane2->updateLocation(latitude, longitude, altitude);
    }else {
        plane* plane = new class plane();
        if(!simMode){
            plane->setSysid(id);
        }else {
            plane->setTeamid(id);
        }
        plane->setLatitude(latitude);
        plane->setLongitude(longitude);
        plane->setAltitude(altitude);
        if (plane != nullptr) {
            m_planes.push_back(plane);
            qDebug() << "plane list changed";
        } else {
            qDebug() << "Error: Attempting to append a null plane object to m_planes.";
        }
    }
    //qDebug() << "changed";
    //emit planesChanged();
}

void PlaneController::updateYaw(int id, float yaw)
{
    for (plane *plane : m_planes) {
        if(plane->teamid() != -1){
            if(plane->teamid() == id){
                plane->setYaw(yaw);
            }
        }else if(plane->sysid() != -1){
            if(plane->sysid() == id){
                plane->setYaw(yaw);
            }
        }
    }
    //emit planesChanged();
}

void PlaneController::updateSpeed(int id, float speed)
{
    struct planestatus a = quickCheck(id);
    if(a.isValid){
        a.plane2->setAirspeed(speed);
    }
}

void PlaneController::updatePressure(int sysid, float pressure)
{
    struct planestatus a = quickCheck(sysid);
    if(a.isValid){
        a.plane2->setPressure(pressure);
    }
}

void PlaneController::updateBattery(int sysid, float voltage, float current, float remainingCapacity)
{
    struct planestatus a = quickCheck(sysid);
    if (a.isValid){
        a.plane2->setVoltage(voltage);
        a.plane2->setCurrent(current);
        a.plane2->setRemainingCapacity(remainingCapacity);
    }
}

void PlaneController::setTeam(int sysid, int teamid)
{
    for (plane *plane : m_planes) {
        if (plane->sysid() == sysid) {
            plane->setTeamid(teamid);
            return;
        }
    }
}

void PlaneController::changeSelection(int systemid)
{
    struct planestatus stat = quickCheck(systemid);
    if(stat.isValid && stat.isLocal){
        qDebug() << "local select";

        stat.plane2->setIsSelected(stat.plane2->sysid() != m_selectedid);
        setSelectedid(stat.plane2->sysid() == m_selectedid ? -3 : stat.plane2->sysid());

    }else if(stat.isValid && !stat.isLocal){
        qDebug() << "tekno select";
        stat.plane2->setIsSelected(stat.plane2->teamid() != m_selectedid);
        setSelectedid(stat.plane2->teamid() == m_selectedid ? -9 : stat.plane2->teamid());
    }
    qDebug() << "Selected id: " << m_selectedid;
    if(m_selectedid >= 0){
        m_notificationCenter->sendNotification(QString("Selected ID: ").append(QString::number(m_selectedid)), NotificationCenter::Secondary);
    }

    for (plane* plane : m_planes){
        if(plane->sysid() != m_selectedid){
            plane->setIsSelected(false);
        }
    }
}

void PlaneController::removeLocalPlane(int sysid)
{
    for(plane* plane : m_planes){
        if(plane->sysid() != -1 && plane->sysid() == sysid){
            removePlane(plane);
        }
    }
}

void PlaneController::throttledUpdate()
{
    emit planesChanged();
}

int PlaneController::selectedid() const
{
    return m_selectedid;
}

void PlaneController::setSelectedid(int newSelectedid)
{
    if (m_selectedid == newSelectedid)
        return;
    m_selectedid = newSelectedid;
    //emit selectedidChanged();
}

plane *PlaneController::findMainPlane(int id, bool tekno)
{
    for(plane* plane : m_planes){
        if(!tekno && plane->sysid() != -1 && plane->sysid() == id){
            return plane;
        }
        if (tekno && plane->teamid() != -1 && plane->teamid() == id){
            return plane;
        }
    }
    return nullptr;
}
