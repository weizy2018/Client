#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "settingwindow.h"
#include <string>
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setColumnWidth(0, 110);
    ui->tableWidget->setColumnWidth(1, 30);
    for (int i = 0; i < 20; i++) {
        int rowcount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowcount);
        QTableWidgetItem * item = new QTableWidgetItem("aaaaaaaa" + QString::number(i));
        ui->tableWidget->setItem(i, 0, item);
        QTableWidgetItem * item2 = new QTableWidgetItem(QString::number(i));
        ui->tableWidget->setItem(i, 1, item2);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
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
*/

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    QTableWidgetItem * item = ui->tableWidget->item(index.row(), 0);
    qDebug() << item->text();
}
