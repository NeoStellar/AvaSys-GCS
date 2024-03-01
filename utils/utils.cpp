#include <mavlink-out/common/mavlink.h>

#include <qdebug.h>

void handleHeartbeat(mavlink_message_t *message);
void printSystemStatus(mavlink_message_t *message);

class utils {

};

void handleHeartbeat(mavlink_message_t *message){
    mavlink_heartbeat_t heartbeat;
    mavlink_msg_heartbeat_decode(message, &heartbeat);
}

void printSystemStatus(mavlink_message_t *message)
{
    mavlink_heartbeat_t heartbeat;
    mavlink_msg_heartbeat_decode(message, &heartbeat);
    qDebug() << &"Sys ID: " [ message->sysid];
    qDebug() << "Got heartbeat from ";
    switch (heartbeat.autopilot) {
    case MAV_AUTOPILOT_GENERIC:
        qDebug() << "generic";
        break;
    case MAV_AUTOPILOT_ARDUPILOTMEGA:
        qDebug() << "Ardupilot";
        break;
    case MAV_AUTOPILOT_PX4:
        qDebug() << "PX4";
        break;
    default:
        qDebug() << "Other";
        break;
    }
    qDebug() << "autopilot\n\n";
    qDebug() << heartbeat.system_status;
}
