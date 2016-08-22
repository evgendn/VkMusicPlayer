#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "vkaudio.h"
#include "vkauth.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    VkAudio* vkAudio_;
    VkAuth* vkAuth_;
    QMediaPlayer* player_;
    QMediaPlaylist* playlist_;

public slots:
    // Заполнение токеном и идшником для получения списка песен.
    void slotInitVkAudio();
    // Заполение таблицы этими песнями.
    void slotFillTable();
};

#endif // MAINWINDOW_H
