#include "vkaudio.h"

VkAudio::VkAudio(QString token, QString userId,
                 QObject *parent) : QObject(parent)
{
    replyJson_ = "";
    try {
        accessToken_ = token;
        userId_ = userId;
        if (accessToken_.isEmpty() == true
            || userId_.isEmpty() == true) {
            throw EmptyParamsException(accessToken_, userId_);
        }

        QUrl url("https://api.vk.com/method/audio.get?");
        QUrlQuery urlQuery(url);
        urlQuery.addQueryItem("owner_id", userId_);
        urlQuery.addQueryItem("access_token", accessToken_);
        url.setQuery(urlQuery);
        audioGet_ = url;

        QNetworkRequest request(audioGet_);
        reply_ = manager_.get(request);

        connect(reply_, SIGNAL(finished()), this, SLOT(slotReadReply()));
        connect(reply_, SIGNAL(finished()), this, SLOT(slotReplyFinished()));

        connect(this, SIGNAL(gotReply()), this, SLOT(slotManipulateJson()));
    }
    catch (EmptyParamsException params) {
        qDebug() << params.what();
    }
}

QVector<QPair<QString, QStringList> > VkAudio::getSongs() const
{
    return songs_;
}

void VkAudio::slotReplyFinished()
{
    reply_->deleteLater();
}

void VkAudio::slotReadReply()
{
    replyJson_ = QString::fromUtf8(reply_->readAll());
    if (replyJson_.isEmpty() == true) {
        qDebug() << "эээ блээт, пустой ответ!";
    }
    else {
        emit gotReply();
    }
}

void VkAudio::slotManipulateJson()
{
    QJsonDocument jsonReply = QJsonDocument::fromJson(replyJson_.toUtf8());
    QJsonObject jsonObject = jsonReply.object();
    QJsonArray jsonArray = jsonObject["response"].toArray();
    int size = jsonArray.size();
    for (int i = 1; i < size; ++i) {
//        qDebug() << jsonArray[i].toObject()["title"].toString();
        // Собираем атрибуты песни в список, а url в строку
        QStringList properties;
        properties.push_back(jsonArray[i].toObject()["artist"].toString());
        properties.push_back(jsonArray[i].toObject()["title"].toString());

        // Продолжительность из секунд нужно
        // перевести в минуты и секунды для красоты.
        QString duration = "";
        int min = 0;
        int sec = jsonArray[i].toObject()["duration"].toDouble();
        min = sec / 60;
        sec %= 60;
        if (sec < 10) {
            duration = QString::number(min) + ":0" + QString::number(sec);
        }
        else {
            duration = QString::number(min) + ":" + QString::number(sec);
        }
        properties.push_back(duration);


        QString url = jsonArray[i].toObject()["url"].toString();
        url = url.split("?")[0];

        songs_.push_back(QPair<QString, QStringList>(url, properties));
    }

    if (songs_.isEmpty() == false) {
        emit haveReadySongs();
    }

}
