#include "httpclient.h"
#include "teknofestproperties.h"
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QEventLoop>
#include <mavlink/plane.h>
#include <mavlink/planecontroller.h>
HttpClient::HttpClient(QObject *parent)
    : QObject{parent}
{
}

void HttpClient::sendGetRequest(const QString &url) {
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this, [ reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "GET request successful";
            //qDebug() << "Response: " << reply->readAll();
        } else {
            qDebug() << "Error occurred: " << reply->errorString();
        }
        reply->deleteLater();
    });
}

void HttpClient::sendPostRequest(const QString &url, const QByteArray &data) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager.post(request, data);
    connect(reply, &QNetworkReply::finished, this, [ reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "POST request successful";
            //qDebug() << "Response: " << reply->readAll();
        } else {
            qDebug() << "Error occurred: " << reply->errorString();
        }
        reply->deleteLater();
    });
}

void HttpClient::sendPostRequest(const QString &url, const QByteArray &data, const QByteArray &sessionId)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Set cookie header
    request.setRawHeader("Cookie", sessionId);

    QNetworkReply *reply = manager.post(request, data);
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "POST request successful";
            //qDebug() << "Response: " << reply->readAll();
        } else {
            qDebug() << "Error occurred 2: " << reply->errorString();
        }
        reply->deleteLater();
    });
}

void HttpClient::sendGetRequest(const QString &url, const QByteArray &jsonData) {
    QUrl requestUrl(url);
    QUrlQuery query;

    // Encode JSON data
    QString encodedJson = QUrl::toPercentEncoding(jsonData);

    // Add JSON data as a query parameter
    query.addQueryItem("json", encodedJson);

    // Append the query to the URL
    requestUrl.setQuery(query);

    QNetworkRequest request(requestUrl);
    QNetworkReply *reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this, [ reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "GET request successful";
            //qDebug() << "Response: " << reply->readAll();
        } else {
            qDebug() << "Error occurred: " << reply->errorString();
        }
        reply->deleteLater();
    });
}

void HttpClient::sendGetRequest(const QString &url, const QByteArray &jsonData, const QByteArray &sessionId)
{
    QUrl requestUrl(url);
    QUrlQuery query;

    // Encode JSON data
    QString encodedJson = QUrl::toPercentEncoding(jsonData);

    // Add JSON data as a query parameter
    query.addQueryItem("json", encodedJson);

    // Append the query to the URL
    requestUrl.setQuery(query);

    QNetworkRequest request(requestUrl);

    // Set cookie header
    request.setRawHeader("Cookie", sessionId);

    QNetworkReply *reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this, [ reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "GET request successful";
            //qDebug() << "Response: " << reply->readAll();
        } else {
            qDebug() << "Error occurred: " << reply->errorString();
        }
        reply->deleteLater();
    });
}

void HttpClient::sendAuthRequest(const QString &url, const QString &kadi, const QString &sifre,  std::function<void(std::pair<int, QByteArray>)> callback)
{
    QUrl requestUrl(url + "/giris");

    // Create a JSON object
    QJsonObject jsonData;
    jsonData["kadi"] = kadi;
    jsonData["sifre"] = sifre;
    // Convert JSON data to string
    QJsonDocument jsonDoc(jsonData);
    QByteArray jsonDataBytes = jsonDoc.toJson(QJsonDocument::Compact);

    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //qDebug() << teknofestProperties->session();
    QNetworkReply *reply = manager.post(request, jsonDataBytes);
    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        std::pair<int, QByteArray> pair;
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "POST request successful";
            // Retrieve cookies
            QList<QNetworkCookie> cookies = manager.cookieJar()->cookiesForUrl(reply->url());
            foreach (const QNetworkCookie &cookie, cookies) {
                if(cookie.name() == "session_id"){
                    pair.second = cookie.value();
                }
                //qDebug() << "Cookie:" << cookie.name() << "=" << cookie.value();
            }

            QByteArray responseData = reply->readAll();
            //qDebug() << "Response: " << responseData;

            // Parse JSON response
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
            if (!responseDoc.isNull() && responseDoc.isObject()) {
                QJsonObject responseObject = responseDoc.object();
                if (responseObject.contains("takim_no")) {
                    int takimNo = responseObject["takim_no"].toInt();
                    //qDebug() << "Takim No:" << takimNo;
                    pair.first = takimNo;
                }
            }
        } else {
            qDebug() << "Error occurred: " << reply->errorString();
        }
        reply->deleteLater();
        callback(pair);


    });
}

std::vector<plane*> HttpClient::handlePostResponse(const QByteArray &response, PlaneController* planeController)
{
    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
    QJsonObject jsonObject = jsonResponse.object();

    // Get the array of plane information
    QJsonArray planeArray = jsonObject["konumBilgileri"].toArray();

    // Vector to store Plane objects
    std::vector<plane *> planes;

    // Iterate over each plane object in the array
    for (const QJsonValue &planeValue : planeArray) {
        QJsonObject planeObject = planeValue.toObject();

        // Create a Plane object
        plane* plane = new class plane();


        //qDebug() << planeObject;
        // Parse plane information and set values accordingly
        plane->setTeamid(planeObject["takim_numarasi"].toInt());
        plane->setIhaYonelme(planeObject["iha_yonelme"].toDouble());
        plane->setSysid(-1);
        plane->setLongitude(planeObject["iha_boylam"].toDouble());
        plane->setLatitude(planeObject["iha_enlem"].toDouble());
        plane->setAltitude(planeObject["iha_irtifa"].toDouble());
        plane->setYaw(planeObject["iha_yonelme"].toDouble());
        qDebug() << "plane selected id:" << planeController->selectedid();
        if(planeController->selectedid() == plane->teamid()){
            planeController->setSelectedid(plane->teamid());
            plane->setIsSelected(true);
        }
        qDebug() << "plane selected teamid 2:" << plane->teamid();
        qDebug() << "is plane selected:" << plane->isSelected();
        //plane->setIsSelected(planeController->findPlane(planeObject["takim_numarasi"].toInt())->isSelected()); // Assuming isSelected is not provided in the JSON

        qDebug() << "Plane yaw after getting all: " << plane->yaw();

        // Parse GPS time
        QJsonObject gpsTimeObject = planeObject["gps_saati"].toObject();
        QDateTime gpsTime(
            QDate::currentDate(),
            QTime(
                gpsTimeObject["saat"].toInt(),
                gpsTimeObject["dakika"].toInt(),
                gpsTimeObject["saniye"].toInt(),
                gpsTimeObject["milisaniye"].toInt()
                )
            );

        // Populate location data
        plane->setGpsSaati(gpsTime);
        plane->setIhaBoylam(planeObject["iha_boylam"].toDouble());
        plane->setIhaEnlem(planeObject["iha_enlem"].toDouble());
        plane->setIhaIrtifa(planeObject["iha_irtifa"].toDouble());
        plane->setIhaDikilme(planeObject["iha_dikilme"].toDouble());

        qDebug() << plane->teamid() << ":"<< plane->ihaYonelme();
        plane->setIhaYatis(planeObject["iha_yatis"].toDouble());
        plane->setIhaHiz(planeObject["iha_hiz"].toDouble());
        plane->setIhaBatarya(planeObject["iha_batarya"].toDouble());
        plane->setIhaKilitlenme(planeObject["iha_kilitlenme"].toInt());
        plane->setIhaOtonom(planeObject["iha_otonom"].toInt());
        plane->setHedefMerkezX(planeObject["hedef_merkez_X"].toInt());
        plane->setHedefMerkezY(planeObject["hedef_merkez_Y"].toInt());
        plane->setHedefGenislik(planeObject["hedef_genislik"].toInt());
        plane->setHedefYukseklik(planeObject["hedef_yukseklik"].toInt());


        plane->setAirspeed(plane->ihaHiz());

        // TODO
        // pressure data is missing
        plane->setPressure(plane->ihaDikilme());

        // Add the plane object to the vector

        qDebug() << "Plane: " << plane->teamid() << " Boylam: " << plane->ihaBoylam();
        planes.push_back(plane);
    }
    return planes;
}

std::pair<int, QByteArray> HttpClient::sendAuthRequest()
{
    QUrl requestUrl(m_url + "/giris");

    // Create a JSON object
    QJsonObject jsonData;
    jsonData["kadi"] = m_username;
    jsonData["sifre"] = m_password;
    // Convert JSON data to string
    QJsonDocument jsonDoc(jsonData);
    QByteArray jsonDataBytes = jsonDoc.toJson(QJsonDocument::Compact);

    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << jsonDoc;
    qDebug() << jsonDataBytes;
    //qDebug() << teknofestProperties->session();
    QNetworkReply *reply = manager.post(request, jsonDataBytes);


    qDebug() << "Waiting for Auth request";
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec(); // Wait until the network request is finished

    qDebug() << "Finished Auth Request";

    std::pair<int, QByteArray> pair;
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "POST request successful";
        // Retrieve cookies
        QList<QNetworkCookie> cookies = manager.cookieJar()->cookiesForUrl(reply->url());
        foreach (const QNetworkCookie &cookie, cookies) {
            if(cookie.name() == "session_id"){
                pair.second = cookie.value();
            }
            //qDebug() << "Cookie:" << cookie.name() << "=" << cookie.value();
        }

        QByteArray responseData = reply->readAll();
        //qDebug() << "Response: " << responseData;

        // Parse JSON response
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        if (!responseDoc.isNull() && responseDoc.isObject()) {
            QJsonObject responseObject = responseDoc.object();
            if (responseObject.contains("takim_no")) {
                int takimNo = responseObject["takim_no"].toInt();
                //qDebug() << "Takim No:" << takimNo;
                pair.first = takimNo;
            }
        }
    } else {
        qDebug() << "Error occurred: " << reply->errorString();
    }
    reply->deleteLater();
    return pair;
}

int HttpClient::sendLocationData(TeknofestProperties* teknofestProperties, PlaneController* planeController, const QJsonObject &jsonObject)
{
    // Construct JSON object with the provided data

    //qDebug()<< teknofestProperties->takimid();
    //qDebug() << jsonObject;
    // Convert JSON object to byte array
    QJsonDocument jsonDoc(jsonObject);
    QByteArray postData = jsonDoc.toJson(QJsonDocument::Compact);


    qDebug() << postData;
    //qDebug() << teknofestProperties->session();
    //qDebug() << teknofestProperties->takimid();
    //qDebug() << "AKDJASKDJA " << teknofestProperties->takimid();


    QNetworkRequest request(m_url + "/telemetri_gonder");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Set cookie header
    request.setRawHeader("Cookie", teknofestProperties->session());

    //qDebug() << request.rawHeaderList();

    QNetworkReply *reply = manager.post(request, postData);

    qDebug() << "Waiting for location data post";
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec(); // Wait until the network request is finished

    qDebug() << "Finished location data post";
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "POST request successful";
        QByteArray message = reply->readAll();
        std::vector<plane*> list = handlePostResponse(message, planeController);

        qDebug() << list.size();

        planeController->setPlanes(list);

        return 0;
        //qDebug() << "Response: " << message;
    } else {
        qDebug() << "Error occurred 2: " << reply->errorString();
    }
    reply->deleteLater();
    return 1;
}

QString HttpClient::url() const
{
    return m_url;
}

void HttpClient::setUrl(const QString &newUrl)
{
    if (m_url == newUrl)
        return;
    m_url = newUrl;
    emit urlChanged();
}

QString HttpClient::username() const
{
    return m_username;
}

void HttpClient::setUsername(const QString &newUsername)
{
    if (m_username == newUsername)
        return;
    m_username = newUsername;
    emit usernameChanged();
}

QString HttpClient::password() const
{
    return m_password;
}

void HttpClient::setPassword(const QString &newPassword)
{
    if (m_password == newPassword)
        return;
    m_password = newPassword;
    emit passwordChanged();
}
