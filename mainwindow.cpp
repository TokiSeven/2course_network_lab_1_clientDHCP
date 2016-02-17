#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cl = new dhcp_client;
    connect(cl, SIGNAL(update_info()), this, SLOT(update_gui()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_release_clicked()
{
    this->cl->send(0);
}

void MainWindow::on_button_discover_clicked()
{
    this->cl->send(1);
}

void MainWindow::on_button_request_clicked()
{
    this->cl->send(2);
}

void MainWindow::update_gui()
{
    ui->info_ip->setText(cl->getServerIp());
    ui->info_nm->setText(cl->getServerNetmask());
}
