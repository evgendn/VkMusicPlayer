#include "vkaudio.h"

VkAudio::VkAudio(QString token, QString userId,
                 QObject *parent) : QObject(parent)
{
    accessToken_ = token;
    userId_ = userId;

    QUrl url("https://api.vk.com/method/audio.get?");
    QUrlQuery urlQuery(url);
    urlQuery.addQueryItem("owner_id", userId_);
    urlQuery.addQueryItem("access_token", accessToken_);
    url.setQuery(urlQuery);
    audioGet_ = url;

    QNetworkRequest request(audioGet_);
    reply_ = manager_.get(request);

    connect(reply_, SIGNAL(finished()), this, SLOT(slotReplyFinished()));
    connect(reply_, SIGNAL(finished()), this, SLOT(slotReadReply()));
}

QString VkAudio::getReplyJson() const
{
    return replyJson_;
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
}
