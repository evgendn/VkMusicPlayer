// Делает авторизацию с запоминанием токена и ид.

#ifndef VKAUTH_H
#define VKAUTH_H

#include <QDebug>
#include <QMainWindow>
#include <QMap>
#include <QString>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QUrl>
#include <QUrlQuery>

class VkAuth : public QMainWindow
{
    Q_OBJECT
public:
    explicit VkAuth(QWidget *parent = 0);
    QString getAccessToken() const;
    QString getUserId() const;
private:
    QString urlAuth_;
    QString accessToken_;
    QString userId_;
    QMap<QString, QString> permissions_;
    QWebEngineView* view_;
signals:
    void haveToken();
//    void unsuccess();
public slots:
    void slotUrlChange(QUrl url);
    void slotCheckToken();
};

#endif // VKAUTH_H
