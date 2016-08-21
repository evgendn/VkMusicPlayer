#include "vkauth.h"

VkAuth::VkAuth(QWidget *parent) : QMainWindow(parent)
{
    urlAuth_ = "https://oauth.vk.com/authorize?"
                "client_id=4720727&"
                "scope=audio,offline&"
                "redirect_uri=https://oauth.vk.com/blank.html&"
                "display=popup&"
                "v=5.53&"
                "response_type=token";
    view_ = new QWebEngineView(parent);
    view_->load(QUrl(this->urlAuth_));
    view_->show();

    connect(view_, SIGNAL(urlChanged(QUrl)), this, SLOT(slotUrlChange(QUrl)));
    connect(view_, SIGNAL(loadFinished(bool)), this, SLOT(slotCheckToken()));
    connect(this, SIGNAL(haveToken()), view_, SLOT(close()));
}

QString VkAuth::getAccessToken() const
{
    return accessToken_;
}

QString VkAuth::getUserId() const
{
    return userId_;
}

void VkAuth::slotUrlChange(QUrl url)
{
    url = url.toString().replace("#", "?");
    accessToken_ = QUrlQuery(url).queryItemValue("access_token");
    userId_ = QUrlQuery(url).queryItemValue("user_id");
}

void VkAuth::slotCheckToken()
{
    QString url = view_->url().toString();
    if (url.contains("access_token") == true) {
        emit haveToken();
//        qDebug() << accessToken_;
    }
}

