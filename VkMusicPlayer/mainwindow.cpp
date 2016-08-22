#include "mainwindow.h"
#include "vkaudio.h"
#include "vkauth.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Настройка плеера.
    player_ = new QMediaPlayer(this);
    player_->setVolume(50);
    playlist_ = new QMediaPlaylist(this);

    // Настройка атрибутов таблицы.
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setGeometry(10, 10, 640, 380);
    ui->tableWidget->setColumnWidth(0, 250);
    ui->tableWidget->setColumnWidth(1, 280);
    ui->tableWidget->setColumnWidth(2, 65);

    QStringList headers;
    headers.push_back("Artist");
    headers.push_back("Title");
    headers.push_back("Duration");
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Авторизация
    vkAuth_ = new VkAuth(this);
    connect(vkAuth_, SIGNAL(haveToken()), this, SLOT(slotInitVkAudio()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete vkAudio_;
}

void MainWindow::slotInitVkAudio()
{
    vkAudio_ = new VkAudio(vkAuth_->getAccessToken(),
                           vkAuth_->getUserId(),
                           this);

    connect(vkAudio_, SIGNAL(haveReadySongs()), this, SLOT(slotFillTable()));
}

void MainWindow::slotFillTable()
{
    int row = 0;
    QVector<QPair<QString, QStringList> > songs = vkAudio_->getSongs();
    QVector<QPair<QString, QStringList> >::const_iterator itr = songs.constBegin();
    // Запись в таблицу, но без url
    for (itr; itr != songs.constEnd(); ++itr) {
        // Добавляем новую строку.
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setRowHeight(row, 35);
        QStringList::const_iterator i = itr->second.constBegin();
        int column = 0;
        // Засовываем в таблицу исполнителя, название и длительность песни.
        for (i; i != itr->second.constEnd(); ++i) {
            QTableWidgetItem* data = new QTableWidgetItem();
            data->setText(*i);
            ui->tableWidget->setItem(row, column, data);
            ++column;
        }
        // Добавляем песню в плейлист.
        playlist_->addMedia(QUrl(itr->first));
        ++row;
    }
    player_->setPlaylist(playlist_);
    playlist_->setCurrentIndex(0);
}
