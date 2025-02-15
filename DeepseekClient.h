/*******************************************************************************
 * @Author : yongheng
 * @Data   : 2025/02/14 00:31
*******************************************************************************/
#pragma once


#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>

class DeepSeekClient : public QObject {
    Q_OBJECT

public:
    explicit DeepSeekClient(QObject* parent = nullptr);
    void sendRequest(const QString& message);

signals:
    void get_message(const QString& message);

private slots:

    void onRequestFinished(QNetworkReply* reply);

private:
    QString api_key_;
    QJsonObject json_;
    QJsonArray json_array_;
    QNetworkRequest request_;
    QNetworkAccessManager* manager_;
};
