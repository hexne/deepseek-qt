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
    void send(const QString& message);

signals:
    void get_message(const QString& message, const QString& response_content);

private slots:

    void onRequestFinished(QNetworkReply* reply);

private:
    QJsonObject json_;
    QJsonArray json_array_;
    QNetworkRequest request_;
    QNetworkAccessManager* manager_;

    struct Config {
        QString model{};
        QString api_key{};
        bool stream{};
    } config_;
};
