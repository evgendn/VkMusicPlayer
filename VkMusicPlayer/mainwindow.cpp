#include "mainwindow.h"
#include "vkaudio.h"
#include "vkauth.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
}
