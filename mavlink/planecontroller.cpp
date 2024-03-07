#include "planecontroller.h"
#include "qdebug.h"
#include "QList"
#include "plane.h"
#include "plane.h"

PlaneController::PlaneController(QObject *parent)
    : QObject(parent), m_selectedid(-1)
{
    qRegisterMetaType<int32_t>("int32_t");
    qRegisterMetaType<std::vector<plane*>>("std::vector<plane*>");
    ptr = &m_planes;
    //m_planes = QList<plane*>();
}
void PlaneController::setQmlContext(QQmlContext *context)
{
    context->setContextProperty("planeController",  this);
    context->setContextProperty("planeList", QVariant::fromValue(m_planes));

}
/*
void PlaneController::changeSelection(int teamid){
    qDebug() << "issued";
    for (plane* plane : m_planes){
        if(plane->teamid() == teamid){
            if(plane->teamid() == m_selectedid){
                plane->setIsSelected(false);
                m_selectedid = -3;
            }else {
                plane->setIsSelected(true);
                m_selectedid = teamid;
            }
        }
        /*if (plane->sysid() != selectedid){
            plane->setIsSelected(false);
            setSelectedid(-5);
        }else {
            plane->setIsSelected(true);
            setSelectedid(plane->sysid());
        }
        qDebug() << "Selected id: " << m_selectedid;
    }
}
    */

void PlaneController::addPlane(int teamid, double latitude, double longitude, int altitude)
{
    //plane *plane = new class plane();
    plane *plan = new plane();
    plan->setTeamid(teamid);
    plan->setLatitude(latitude);
    plan->setLongitude(longitude);
    plan->setAltitude(altitude);
    if (plan != nullptr) {
        m_planes.push_back(plan);
        qDebug() << "plane list changed";
        emit planesChanged();
    } else {
        qDebug() << "Error: Attempting to append a null plane object to m_planes.";
    }
    //connect(plane, &plane::locationChanged, this, &PlaneController::locationChanged);
}

void PlaneController::locationChanged(int teamid, float longitude, int latitude, int32_t altitude){
    addOrUpdatePlane(teamid, latitude, longitude, altitude);
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
    qDebug() << "planes size: " << m_planes.size();
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
    //qDebug() << sysid;
    for (plane* plane : m_planes) {
        if(plane->teamid() == -1){
            plane->setGpsSaati(QDateTime::currentDateTime());
            qDebug() << "plane: " << plane->sysid();
            return plane;
        }else if (plane->teamid() == id) {
            //qDebug() << plane->locationData().hedefMerkezY;
            plane->setGpsSaati(QDateTime::currentDateTime());
            qDebug() << "plane team: " << plane->teamid();
            return plane;
        }else {
            qDebug() << "tf is this: " << plane->yaw();
        }
    }
    return nullptr;
}

void PlaneController::removePlane(plane* plane)
{
   ptr->erase(std::remove(m_planes.begin(), m_planes.end(), plane), m_planes.end());
}

void PlaneController::updatePlane(int sysid, double latitude, double longitude, int altitude)
{
    for (plane *plane : m_planes) {
        if (plane->sysid() == sysid) {
            //plane->setIsSelected(findSelected()->sysid() == sysid);
            plane->updateLocation(latitude, longitude, altitude);
            return;
        }
    }
}

void PlaneController::addOrUpdatePlane(int teamid, double latitude, double longitude, int altitude)
{
    bool found = false;
    for (plane *p : m_planes) {
        if (p->getUniqueIdentifier() == teamid) {
            //qDebug() << "Updating plane: " << sysid << " from m_planes";
            updatePlane(teamid, latitude, longitude, altitude);
            p->setGpsSaati(QDateTime::currentDateTime());
            found = true;
            break;
        }
        //qDebug() << "Sysid: " << p->sysid();
        //qDebug() << "Yaw: " << p->yaw();
        if(p->getUniqueIdentifier() == 1){
            qDebug() << "Found ghost plane! Sysid: " << p->sysid();
            removePlane(p);
        }
    }

    if (!found) {
        qDebug() << "Adding plane: " << teamid << " to m_planes";
        addPlane(teamid, latitude, longitude, altitude);
    }


    for (plane *plane : m_planes) {
        if (plane->getUniqueIdentifier() == teamid) {
            //qDebug() << "latitude: " << plane->latitude();
            //qDebug() << "longitude: " << plane->longitude();
            //qDebug() << "altitude: " << plane->altitude();
            //qDebug() << "Got coordinate data from " << plane->sysid();
            return;
        }
    }
}

void PlaneController::AoULocalPlane(int sysid, double latitude, double longitude, int altitude)
{

    //qDebug() << "cu";
    bool found = false;
    for (plane *p : m_planes) {
        if(p->teamid() != -1){
            found = true;
            updateTeknoPlane(p->teamid(), latitude, longitude, altitude);
            break;
        }
        if (p->sysid() == sysid) {
            //qDebug() << "Updating plane: " << sysid << " from m_planes";
            updateLocalPlane(sysid, latitude, longitude, altitude);
            p->setGpsSaati(QDateTime::currentDateTime());
            found = true;
            break;
        }
        emit planesChanged();
        //qDebug() << "Sysid: " << p->sysid();
        //qDebug() << "Yaw: " << p->yaw();
        if(p->sysid() == 1){
            qDebug() << "Found ghost plane! Sysid: " << p->sysid();
            //removePlane(p);
        }
    }

    if (!found) {
        qDebug() << "Adding local plane: " << sysid << " to m_planes";
        addLocalPlane(sysid, latitude, longitude, altitude);
    }


    for (plane *plane : m_planes) {
        if (plane->sysid() == sysid) {
            //qDebug() << "latitude: " << plane->latitude();
            //qDebug() << "longitude: " << plane->longitude();
            //qDebug() << "altitude: " << plane->altitude();
            //qDebug() << "Got coordinate data from " << plane->sysid();
            return;
        }
    }
}
void PlaneController::updateLocalPlane(int sysid, double latitude, double longitude, int altitude)
{
    for (plane *plane : m_planes) {
        if (plane->sysid() == sysid) {
            //qDebug() << "found";
            //plane->setIsSelected(findSelected()->sysid() == sysid);
            plane->updateLocation(latitude, longitude, altitude);
            emit planesChanged();
            return;
        }
    }
}

void PlaneController::updateTeknoPlane(int teamid, double latitude, double longitude, int altitude)
{
    for (plane *plane : m_planes) {
        if (plane->teamid() == teamid) {
            //plane->setIsSelected(findSelected()->sysid() == sysid);
            plane->updateLocation(latitude, longitude, altitude);
            emit planesChanged();
            return;
        }
    }
}
void PlaneController::AoUTeknoPlane(int teamid, double latitude, double longitude, int altitude)
{
    bool found = false;
    for (plane *p : m_planes) {
        if (p->teamid() == teamid) {
            //qDebug() << "Updating plane: " << sysid << " from m_planes";
            updateTeknoPlane(teamid, latitude, longitude, altitude);
            p->setGpsSaati(QDateTime::currentDateTime());
            found = true;
            break;
        }
        //qDebug() << "Sysid: " << p->sysid();
        //qDebug() << "Yaw: " << p->yaw();
        if(p->teamid() == 1){
            qDebug() << "Found ghost plane! Sysid: " << p->sysid();
            removePlane(p);
        }
    }

    if (!found) {
        qDebug() << "Adding plane: " << teamid << " to m_planes";
        addTeknoPlane(teamid, latitude, longitude, altitude);
    }


    for (plane *plane : m_planes) {
        if (plane->teamid() == teamid) {
            //qDebug() << "latitude: " << plane->latitude();
            //qDebug() << "longitude: " << plane->longitude();
            //qDebug() << "altitude: " << plane->altitude();
            //qDebug() << "Got coordinate data from " << plane->sysid();
            return;
        }
    }
}

void PlaneController::addLocalPlane(int sysid, double latitude, double longitude, int altitude)
{
    plane* plane = new class plane();
    plane->setSysid(sysid);
    plane->setTeamid(-1);
    plane->setLatitude(latitude);
    plane->setLongitude(longitude);
    plane->setAltitude(altitude);
    if (plane != nullptr) {
        m_planes.push_back(plane);
        qDebug() << "plane list changed";
        emit planesChanged();
    } else {
        qDebug() << "Error: Attempting to append a null plane object to m_planes.";
    }
}

void PlaneController::addTeknoPlane(int teamid, double latitude, double longitude, int altitude)
{
    plane* plane = new class plane();
    plane->setSysid(-1);
    plane->setTeamid(teamid);
    plane->setLatitude(latitude);
    plane->setLongitude(longitude);
    plane->setAltitude(altitude);
    if (plane != nullptr) {
        m_planes.push_back(plane);
        qDebug() << "plane list changed";
        emit planesChanged();
    } else {
        qDebug() << "Error: Attempting to append a null plane object to m_planes.";
    }
}

void PlaneController::updateYaw(int teamid, float yaw)
{
    for (plane *plane : m_planes) {
        if (plane->getUniqueIdentifier() == teamid) {
            plane->setGpsSaati(QDateTime::currentDateTime());
            plane->setYaw(yaw);
            emit planesChanged();
            return;
        }
    }
}



void PlaneController::updateLocalYaw(int sysid, float yaw)
{
    for (plane *plane : m_planes) {
        if(plane->teamid() != -1) {
            updateTeknoYaw(plane->teamid(), yaw);
        }
        if (plane->sysid() == sysid) {
            plane->setGpsSaati(QDateTime::currentDateTime());
            plane->setYaw(yaw);
            emit planesChanged();
            return;
        }
    }
}

void PlaneController::updateTeknoYaw(int teamid, float yaw)
{
    for (plane *plane : m_planes) {
        if (plane->teamid() == teamid) {
            plane->setGpsSaati(QDateTime::currentDateTime());
            plane->setYaw(yaw);
            emit planesChanged();
            return;
        }
    }
}

void PlaneController::updateLocalSpeed(int sysid, float speed)
{
    //qDebug() << "updating local speed";
    for (plane* plane : m_planes){
        if(plane->sysid() == sysid){
            //qDebug() << "found plane";
            plane->setAirspeed(speed);
            emit planesChanged();
            return;
        }
    }
}

void PlaneController::updateLocalPressure(int sysid, float pressure)
{
    for(plane* plane: m_planes){
        if(plane->sysid() == sysid){
            plane->setPressure(pressure);
            emit planesChanged();
        }
    }
}

void PlaneController::changeLocalSelection(int sysid)
{
    qDebug() << "issued";
    for (plane* plane : m_planes){
        plane->setIsSelected(false);
        if(plane->sysid() == sysid){
            if(plane->sysid() == m_selectedid){
                plane->setIsSelected(false);
                m_selectedid = -3;
            }else {
                plane->setIsSelected(true);
                m_selectedid = plane->sysid();
            }
        }
        /*if (plane->sysid() != selectedid){
            plane->setIsSelected(false);
            setSelectedid(-5);
        }else {
            plane->setIsSelected(true);
            setSelectedid(plane->sysid());
        } */
        //qDebug() << "Selected id: " << m_selectedid;
    }
    emit planesChanged();
    qDebug() << "Selected id: " << sysid;
}

void PlaneController::changeTeknoSelection(int teamid)
{
    qDebug() << "issued";
    for (plane* plane : m_planes){
         if(plane->sysid() != -1){
            changeLocalSelection(plane->sysid());
            return;
        }
        if(plane->teamid() == teamid){
            if(plane->teamid() == m_selectedid){
                plane->setIsSelected(false);
                m_selectedid = -9;
            }else {
                plane->setIsSelected(true);
                m_selectedid = plane->teamid();
            }
        }
        /*if (plane->sysid() != selectedid){
            plane->setIsSelected(false);
            setSelectedid(-5);
        }else {
            plane->setIsSelected(true);
            setSelectedid(plane->sysid());
        } */
        emit planesChanged();
        qDebug() << "Selected id: " << m_selectedid;
    }
}

void PlaneController::setTeam(int sysid, int teamid)
{
    for (plane *plane : m_planes) {
        if (plane->sysid() == sysid) {
            //qDebug() << "setTEam: " << teamid;
            plane->setTeamid(teamid);
            return;
        }
    }
}

void PlaneController::changeSelection(int systemid, int teamid)
{
    if(teamid != -1){
        qDebug() << "tekno select";
        changeTeknoSelection(teamid);
    }else {
        qDebug() << "local select";
        changeLocalSelection(systemid);
    }
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

plane *PlaneController::findMainPlane(int id)
{
    for(plane* plane : m_planes){
        if(plane->sysid() != -1 && plane->sysid() == id){
            return plane;
        }
    }
    return nullptr;
}
