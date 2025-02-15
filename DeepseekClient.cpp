/*******************************************************************************
 * @Author : yongheng
 * @Data   : 2025/02/14 00:31
*******************************************************************************/

#include "DeepseekClient.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <iostream>

DeepSeekClient::DeepSeekClient(QObject* parent) : QObject(parent) {
    QFile file("./api_key.txt");

    if (!file.open(QIODevice::ReadOnly))
        throw std::runtime_error("Failed to open file");

    api_key_ = file.readAll();

    QUrl url("https://api.deepseek.com/chat/completions");
    request_.setUrl(url);
    request_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request_.setRawHeader("Authorization", ("Bearer " + api_key_).toUtf8());

    manager_ = new QNetworkAccessManager(this);
    connect(manager_, &QNetworkAccessManager::finished, this, &DeepSeekClient::onRequestFinished);

    json_["model"] = "deepseek-chat";
    json_["stream"] = false;
}

void DeepSeekClient::sendRequest(const QString& message) {
    QJsonObject obj;
    obj["role"] = "user";
    obj["content"] = message;
    json_array_.append(obj);

    json_["messages"] = json_array_;

    QJsonDocument doc(json_);
    QByteArray data = doc.toJson();
    qDebug() << doc;

    // 发送 POST 请求
    manager_->post(request_, data);
}

void DeepSeekClient::onRequestFinished(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        // 读取响应数据
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

        if (!responseDoc.isNull()) {
            // 解析 JSON 响应
            QJsonObject root = responseDoc.object();
            QJsonArray choices = root["choices"].toArray();
            if (!choices.isEmpty()) {
                QJsonObject choice = choices[0].toObject();
                QJsonObject message = choice["message"].toObject();
                QString content = message["content"].toString();
                emit get_message(content);
                qDebug() << "Response Content:" << content;
            }
        } else {
            qDebug() << "Failed to parse JSON response.";
        }
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();
}