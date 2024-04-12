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
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged FINAL)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged FINAL)
public:
    explicit HttpClient(QObject *parent = nullptr);
    void sendPostRequest(const QString &url, const QByteArray &data);
    void sendPostRequest(const QString &url, const QByteArray &data, const QByteArray &sessionId);
    void sendGetRequest(const QString &url);
    void sendGetRequest(const QString &url, const QByteArray &jsonData = QByteArray());
    void sendGetRequest(const QString &url, const QByteArray &jsonData, const QByteArray &sessionId);
    void sendAuthRequest(const QString &url, TeknofestProperties *teknofestProperties, const QString &kadi, const QString &sifre);
    std::pair<int, QByteArray> sendAuthRequest();
    void sendAuthRequest(const QString &url, const QString &kadi, const QString &sifre, std::function<void (std::pair<int, QByteArray>)> callback);
    int sendLocationData(TeknofestProperties *teknofestProperties, PlaneController *planeController, const QJsonObject &jsonData);
    std::vector<plane *> handlePostResponse(const QByteArray &response, PlaneController *planeController);
    QString url() const;
    void setUrl(const QString &newUrl);

    QString username() const;
    void setUsername(const QString &newUsername);

    QString password() const;
    void setPassword(const QString &newPassword);

signals:
    void urlChanged();

    void usernameChanged();

    void passwordChanged();

private:
    QNetworkAccessManager manager;
    QString m_url;
    QString m_username;
    QString m_password;
};

#endif // HTTPCLIENT_H
