#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "settingwindow.h"
#include <string>
#include <QDebug>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "utils.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setColumnWidth(0, 110);
    ui->tableWidget->setColumnWidth(1, 30);

    ui->chatingFriend->setText("");

    //处理客户端发来的消息
    pthread_t pid;
    pthread_create(&pid, 0, getServerMsg, this);


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

//好友列表点击响应
void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    QTableWidgetItem * item = ui->tableWidget->item(index.row(), 0);
    ui->chatingFriend->setText(item->text());
    qDebug() << item->text();
}

//发送按钮响应
void MainWindow::on_pushButton_clicked()
{
    if (ui->chatingFriend->text() != "") {
        QString msg = ui->plainTextEdit->toPlainText();
        string currentTime = Utils::getCurrentTime();
        qDebug() << QString::fromStdString(currentTime);

        ui->plainTextEdit_2->appendPlainText(QString::fromStdString(currentTime));
        ui->plainTextEdit_2->appendPlainText(msg);
        ui->plainTextEdit_2->appendPlainText("");

        ui->plainTextEdit->clear();

        SendContent content;
        content.type = SEND_MESSAGE_ACTION;
        strcpy(content.sender, Utils::getInstance()->user);
        strcpy(content.receiver, ui->chatingFriend->text().toStdString().c_str());
        strcpy(content.message, msg.toStdString().c_str());
        strcpy(content.sendTime, currentTime.c_str());
        //write(Utils::getInstance()->sockedfd, &content, sizeof(content));
        write(Utils::getInstance()->sockedfd, &content, sizeof(content));
    }

}

//获取用户列表
int getFriendsList(MainWindow * window) {

    friends f;
    if (read(Utils::getInstance()->sockedfd, &f, sizeof(f)) == 0) {
        return -1;
    }
    while (f.flag != END_FLAG) {
        qDebug(f.friendid);
        int rowcount = window->ui->tableWidget->rowCount();
        window->ui->tableWidget->insertRow(rowcount);
        QTableWidgetItem * item = new QTableWidgetItem(f.friendid);
        window->ui->tableWidget->setItem(rowcount, 0, item);
        QTableWidgetItem * item2 = new QTableWidgetItem("");
        window->ui->tableWidget->setItem(rowcount, 1, item2);
        if (read(Utils::getInstance()->sockedfd, &f, sizeof(f)) == 0) {
            return -1;
        }
    }
    return 0;
}

//接受服务器端发来的消息
void * getServerMsg(void * w) {
    MainWindow * window = (MainWindow*)w;
    getFriendsList(window);


}







