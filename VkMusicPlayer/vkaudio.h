// Получает json с аудиозаписями.

#ifndef VKAUDIO_H
#define VKAUDIO_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariant>

#include "vkauth.h"

class VkAudio : public QObject
{
    Q_OBJECT
public:
    explicit VkAudio(QString token, QString userId, QObject *parent = 0);
    QString getReplyJson() const;
private:
    QString accessToken_;
    QString userId_;
    QNetworkAccessManager manager_;
    QNetworkReply* reply_;
    QUrl audioGet_;
    QString replyJson_;
public slots:
    void slotReplyFinished();
    void slotReadReply();
};

#endif // VKAUDIO_H
