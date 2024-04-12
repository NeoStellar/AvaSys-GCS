#ifndef NOTIFICATIONCENTER_H
#define NOTIFICATIONCENTER_H

#include <QObject>

class NotificationCenter : public QObject
{
    Q_OBJECT
public:
    explicit NotificationCenter(QObject *parent = nullptr);

    enum NotificationCategory {
        Success = 4,
        Error = 5,
        Info = 3,
        Primary = 1,
        Secondary = 2,
        Fatal = 6
    };
    Q_ENUM(NotificationCategory)

    //Q_INVOKABLE void sendNotification(QString text);
    Q_INVOKABLE void sendNotification(QString text, NotificationCategory category = Primary);
signals:
    void newNotification(const QString &text, const NotificationCategory &category);
};


#endif // NOTIFICATIONCENTER_H
