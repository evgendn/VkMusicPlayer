#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

public slots:
    void slotInitVkAudio();
    void slotFillTable();
};

#endif // MAINWINDOW_H
