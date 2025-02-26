/*******************************************************************************
 * @Author : yongheng
 * @Data   : 2025/02/14 00:31
*******************************************************************************/

#include "DeepseekClient.h"
#include "MainWindow.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <iostream>

DeepSeekClient::DeepSeekClient(QObject* parent) : QObject(parent) {
    QFile file("./api_key.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Failed to open file");
    }
    api_key_ = file.readAll();

    QFile config_file("./config.json");
    if (!config_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Failed to open config.json");
    }
    QByteArray config_json_data = config_file.readAll();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(config_json_data, &parseError);
    QJsonObject config_json = jsonDoc.object();
    config_.model = config_json["model"].toString();
    config_.stream = config_json["stream"].toBool();

    QUrl url("https://api.deepseek.com/chat/completions");
    request_.setUrl(url);
    request_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request_.setRawHeader("Authorization", ("Bearer " + api_key_).toUtf8());

    manager_ = new QNetworkAccessManager(this);
    connect(manager_, &QNetworkAccessManager::finished, this, &DeepSeekClient::onRequestFinished);

    json_["model"] = config_.model;
    json_["stream"] = config_.stream;
}

void DeepSeekClient::send(const QString& message) {
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
                QString response_content = message["reasoning_content"].toString();

                QJsonObject obj;
                obj["role"] = "system";
                obj["content"] = content;
                json_array_.append(obj);

                emit get_message(content,response_content);
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