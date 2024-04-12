#include "notificationcenter.h"



NotificationCenter::NotificationCenter(QObject *parent) : QObject(parent)
{

}

void NotificationCenter::sendNotification(QString text, NotificationCenter::NotificationCategory category)
{
    emit newNotification(text, category);
}
