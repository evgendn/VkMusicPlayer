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
    // Пикает, когда получили токен.
    void haveToken();
public slots:
    // Забираем токен и ид-шник из url.
    void slotUrlChange(QUrl url);
    // Смотрим токен(пустой или нет) и пускаем сигнал haveToken()
    void slotCheckToken();
};

#endif // VKAUTH_H
