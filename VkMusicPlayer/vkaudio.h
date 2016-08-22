// Получает json с аудиозаписями и преобразовывет его в QQueue<QPair<...>>.

#ifndef VKAUDIO_H
#define VKAUDIO_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPair>
#include <QQueue>
#include <QStringList>

#include "vkauth.h"

class VkAudio : public QObject
{
    Q_OBJECT
public:
    explicit VkAudio(QString token, QString userId, QObject *parent = 0);
    QVector<QPair<QString, QStringList> > getSongs() const;
private:
    QString accessToken_;
    QString userId_;
    QNetworkAccessManager manager_;
    QNetworkReply* reply_;
    QUrl audioGet_;
    QString replyJson_;
    // Я ебал, была же вроде unsorted map в 4.8.
    QVector<QPair<QString, QStringList> > songs_;
    class EmptyParamsException
    {
    public:
        EmptyParamsException(QString str1="", QString str2="") {
            arg1_ = str1;
            arg2_ = str2;
        }

        QString what() const{
            return QString("EmptyParamsException: Bad arguments(accessToken = "
                           + arg1_ + " userId = " + arg2_ + ")");
        }
    private:
        QString arg1_;
        QString arg2_;
    };

private slots:
    // Удаляем reply, когда он больше не нужен.
    void slotReplyFinished();
    // Получем ответ(json) и пускаем сигнал gotReply().
    void slotReadReply();
    // Пакуем атрибуты из json-на в вектор и пускаем сигнал haveReadySongs().
    void slotManipulateJson();

signals:
    // Пикаем, когда получили ответ.
    void gotReply();
    // Пикаем, когда запаковали вектор.
    void haveReadySongs();
};

#endif // VKAUDIO_H
