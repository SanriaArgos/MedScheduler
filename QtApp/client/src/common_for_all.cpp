#include "common_for_all.hpp"
#include <QNetworkAccessManager>
#include <string>
#include <QString>
#include <QEventLoop>
#include <QNetworkReply>
#include <QtNetwork>
#include <QtGui>
#include <QtCore>
#include <QAbstractEventDispatcher>
const std::string base_url = "https://api.medscheduler.ru";

std::string send_get_request(const std::string &url) {
    QNetworkAccessManager Manager;
    QUrl api_url(QString::fromStdString(url));
    QNetworkRequest request(api_url);
    request.setTransferTimeout(10000);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = Manager.get(request);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop,&QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        std::string error = "HTTP Error: " + reply->errorString().toStdString();
        reply->deleteLater();
        std::cerr<<error<<std::endl;
    }
    QString Response = reply->readAll();
    return Response.toStdString();
}

std::string send_post_request(const std::string &url, const nlohmann::json &json_data) {
    QNetworkAccessManager Manager;
    QUrl api_url(QString::fromStdString(url));
    QNetworkRequest request(api_url);
    request.setTransferTimeout(10000);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray post_data = QByteArray::fromStdString(json_data.dump());
    QNetworkReply *reply = Manager.post(request,post_data);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop,&QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        std::string error = "HTTP Error: " + reply->errorString().toStdString();
        reply->deleteLater();
        std::cerr<<error<<std::endl;
    }
    QString Response = reply->readAll();
    return Response.toStdString();
}

std::string send_delete_request(const std::string &url) {
    QNetworkAccessManager Manager;
    QUrl api_url(QString::fromStdString(url));
    QNetworkRequest request(api_url);
    request.setTransferTimeout(10000);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = Manager.deleteResource(request);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop,&QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        std::string error = "HTTP Error: " + reply->errorString().toStdString();
        reply->deleteLater();
        std::cerr<<error<<std::endl;
    }
    QString Response = reply->readAll();
    return Response.toStdString();
}

std::string
send_delete_request(const std::string &url, const nlohmann::json &json_data) {
    QNetworkAccessManager Manager;
    QUrl api_url(QString::fromStdString(url));
    QNetworkRequest request(api_url);
    request.setTransferTimeout(10000);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray patch_data = QByteArray::fromStdString(json_data.dump());
    QBuffer *buffer = new QBuffer;
    buffer->setData(patch_data);
    buffer->open(QIODevice::ReadOnly);
    QNetworkReply *reply = Manager.sendCustomRequest(request, "DELETE", buffer);
    buffer->setParent(reply);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop,&QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        std::string error = "HTTP Error: " + reply->errorString().toStdString();
        reply->deleteLater();
        std::cerr<<error<<std::endl;
    }
    QString Response = reply->readAll();
    return Response.toStdString();
}

std::string
send_patch_request(const std::string &url, const nlohmann::json &json_data) {
    QNetworkAccessManager Manager;
    QUrl api_url(QString::fromStdString(url));
    QNetworkRequest request(api_url);
    request.setTransferTimeout(10000);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray patch_data = QByteArray::fromStdString(json_data.dump());
    QBuffer *buffer = new QBuffer;
    buffer->setData(patch_data);
    buffer->open(QIODevice::ReadOnly);
    QNetworkReply *reply = Manager.sendCustomRequest(request, "PATCH", buffer);
    buffer->setParent(reply);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop,&QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        std::string error = "HTTP Error: " + reply->errorString().toStdString();
        reply->deleteLater();
        std::cerr<<error<<std::endl;
    }
    QString Response = reply->readAll();
    return Response.toStdString();
}
