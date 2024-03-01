#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "teknofestproperties.h"

#include <QNetworkAccessManager>
#include <QObject>

#include <mavlink/plane.h>
#include <mavlink/planecontroller.h>

class HttpClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = nullptr);
    void sendPostRequest(const QString &url, const QByteArray &data);
    void sendPostRequest(const QString &url, const QByteArray &data, const QByteArray &sessionId);
    void sendGetRequest(const QString &url);
    void sendGetRequest(const QString &url, const QByteArray &jsonData = QByteArray());
    void sendGetRequest(const QString &url, const QByteArray &jsonData, const QByteArray &sessionId);
    void sendAuthRequest(const QString &url, TeknofestProperties *teknofestProperties, const QString &kadi, const QString &sifre);
    std::pair<int, QByteArray> sendAuthRequest(const QString &url, const QString &kadi, const QString &sifre);
    void sendAuthRequest(const QString &url, const QString &kadi, const QString &sifre, std::function<void (std::pair<int, QByteArray>)> callback);
    void sendLocationData(const QString &url, TeknofestProperties *teknofestProperties, PlaneController *planeController, const QJsonObject &jsonData);
    std::vector<plane *> handlePostResponse(const QByteArray &response, PlaneController *planeController);
signals:
private:
    QNetworkAccessManager manager;
};

#endif // HTTPCLIENT_H
