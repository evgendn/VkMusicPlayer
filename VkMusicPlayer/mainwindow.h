#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSlider>
#include <QTableWidgetItem>
#include <QTableWidgetSelectionRange>

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
    qint64 duration;

public slots:
    // Заполнение токеном и идшником для получения списка песен.
    void slotInitVkAudio();
    // Заполение таблицы этими песнями.
    void slotFillTable();
    // Играется песня при двойном клике на строке таблицы.
    void slotPlaySong(QTableWidgetItem* tableItem);
    void slotChangeTrackValue(qint64 data64);
    // Устанавливаем sliderTrack.range и поле duration этого класса.
    void slotSetDuration(qint64 data64);
    // Перемотка песни.
    void slotSetPosition(int data);
    // "Направление" следующей песни.
    void slotPlayNextSong(int index);
    void slotButtonNextSong();
    void slotButtonPreviousSong();

signals:
    void sendInt(int);
    void sendQInt64(qint64);
    void playNextSong();
    void playPreviousSong();
};

#endif // MAINWINDOW_H
