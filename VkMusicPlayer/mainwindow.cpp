#include "mainwindow.h"
#include "vkaudio.h"
#include "vkauth.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isPlay = false;

    // Настройка плеера.
    player_ = new QMediaPlayer(this);
    player_->setVolume(50);
    playlist_ = new QMediaPlaylist(this);
    duration = 0;

    ui->sliderVolume->setRange(0, 100);
    ui->sliderVolume->setSliderPosition(50);
    ui->sliderVolume->setSingleStep(10);

    ui->sliderTrack->setSliderPosition(0);
    ui->sliderTrack->setSingleStep(10);

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

    // Выделение всей строки и всех колонок, а не одной клетки.
    connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this, SLOT(slotPlaySong(QTableWidgetItem*)));

    // Настройка громкости.
    connect(ui->sliderVolume, SIGNAL(valueChanged(int)),
            player_, SLOT(setVolume(int)));

    // Переводим position из qint64 to int и
    // и меняем slider value.
    connect(player_, SIGNAL(positionChanged(qint64)),
            this, SLOT(slotChangeTrackValue(qint64)));
    connect(this, SIGNAL(sendInt(int)),
            ui->sliderTrack, SLOT(setValue(int)));

    // Ловим продолжительность песни.
    connect(player_, SIGNAL(durationChanged(qint64)),
            this, SLOT(slotSetDuration(qint64)));

    // Перематываем с помощью slider и
    // переводим int to qint64 для player.position.
    connect(ui->sliderTrack, SIGNAL(sliderMoved(int)),
            this, SLOT(slotSetPosition(int)));
    connect(this, SIGNAL(sendQInt64(qint64)),
            player_, SLOT(setPosition(qint64)));

    // Переход на следующую песню(автоматически).
    connect(playlist_, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotPlayNextSong(int)));
    // Управление с помощью кнопок.
    connect(ui->buttonPlayPause, SIGNAL(pressed()),
            this, SLOT(slotButtonPressed()));
    connect(ui->buttonNext, SIGNAL(pressed()),
            this, SLOT(slotButtonNextSong()));
    connect(ui->buttonPrevious, SIGNAL(pressed()),
            this, SLOT(slotButtonPreviousSong()));
    connect(ui->buttonStop, SIGNAL(pressed()),
            player_, SLOT(stop()));
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
                           this
                           );

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
            data->setFlags(data->flags() ^ Qt::ItemIsEditable);
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

void MainWindow::slotPlaySong(QTableWidgetItem* tableItem)
{
    // Выделение всей строки и всех колонок, а не одной клетки.
    ui->tableWidget->setRangeSelected(QTableWidgetSelectionRange(
                                          tableItem->row(), 0,
                                          tableItem->row(), 2), true
                                      );

    playlist_->setCurrentIndex(tableItem->row());
//    player_->play();
//    isPlay = true;
    slotButtonPressed();
}

void MainWindow::slotChangeTrackValue(qint64 data64)
{
    int data = static_cast<int>(data64);
//    qDebug() << data;
    emit sendInt(data);
}

void MainWindow::slotSetDuration(qint64 data64)
{
    duration = static_cast<int>(data64);
    ui->sliderTrack->setRange(0, duration);
    //    qDebug() << duration;
}

void MainWindow::slotSetPosition(int data)
{
    qint64 data64 = static_cast<qint64>(data);
    emit sendQInt64(data64);
}

void MainWindow::slotPlayNextSong(int index)
{
    ui->tableWidget->selectRow(index);
}

void MainWindow::slotButtonNextSong()
{
    int index = playlist_->nextIndex();
    ui->tableWidget->selectRow(index);
    playlist_->next();
}

void MainWindow::slotButtonPreviousSong()
{
    int index = playlist_->previousIndex();
    ui->tableWidget->selectRow(index);
    playlist_->previous();
}

void MainWindow::slotButtonPressed()
{
    if (isPlay == true) {
        player_->pause();
        ui->buttonPlayPause->setIcon(QIcon(":/images/images/play.png"));
        ui->buttonPlayPause->setIconSize(QSize(70, 70));
        isPlay = false;
    }
    else {
        player_->play();
        ui->buttonPlayPause->setIcon(QIcon(":/images/images/pause.png"));
        ui->buttonPlayPause->setIconSize(QSize(70, 70));
        isPlay = true;
    }
}
