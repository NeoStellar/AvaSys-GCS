#ifndef MAVLINKUDP_H
#define MAVLINKUDP_H

#include <QObject>

class mavlinkudp : public QObject
{
    Q_OBJECT

public:
    explicit mavlinkudp(QObject *parent = nullptr);

signals:
};

#endif // MAVLINKUDP_H
