#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "settingwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginPage_clicked()
{
    LoginWindow * login = new LoginWindow;
    login->show();
    delete this;
}

void MainWindow::on_settingPage_clicked()
{
    SettingWindow * set = new SettingWindow(this);
    set->show();
    this->hide();
}
