#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "settingwindow.h"
#include <string>
#include <QDebug>
#include <QScrollBar>

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
    thread = new ReadThread(this);
    thread->start();

    connect(thread, SIGNAL(sendText(QString)), this, SLOT(appendText(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete thread;
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
    QTableWidgetItem * item1 = ui->tableWidget->item(index.row(), 0);
    QTableWidgetItem * item2 = ui->tableWidget->item(index.row(), 1);

    if (item1->text() != ui->chatingFriend->text() && ui->chatingFriend->text() != "") {
        dialogue[ui->chatingFriend->text().toStdString()] = ui->plainTextEdit_2->toPlainText();
    }

    if (item1->text() != ui->chatingFriend->text()) {
        ui->chatingFriend->setText(item1->text());
        qDebug() << item1->text();


        ui->plainTextEdit_2->setPlainText(dialogue[item1->text().toStdString()]);

        //滑动滚动条到文本末尾
        QScrollBar * sc = ui->plainTextEdit_2->verticalScrollBar();
        if (sc) {
            sc->setSliderPosition(sc->maximum());

        }

        if (item2->text() != "") {
            item2->setText("");
            count[item1->text().toStdString()] = 0;
        }
    }

}

//发送按钮响应
void MainWindow::on_pushButton_clicked()
{
    if (ui->chatingFriend->text() != "") {
        QString msg = ui->plainTextEdit->toPlainText();
        string currentTime = Utils::getCurrentTime();
        qDebug() << QString::fromStdString(currentTime);

        SendContent content;
        content.type = SEND_MESSAGE_ACTION;
        strcpy(content.sender, Utils::getInstance()->user);
        strcpy(content.receiver, ui->chatingFriend->text().toStdString().c_str());

        QByteArray m = msg.toLocal8Bit();

        strcpy(content.message, string(m).c_str());
        strcpy(content.sendTime, currentTime.c_str());

        int a = write(Utils::getInstance()->sockedfd, &content, sizeof(content));
        if (a < 0) {
            qDebug() << QString::fromUtf8("发送失败");
            ui->statusBar->showMessage(QString::fromUtf8("发送失败，请检查网络后重试"), 5000);

        } else {
            ui->plainTextEdit_2->appendPlainText(QString::fromUtf8("\n我  ") + QString::fromStdString(currentTime));
            ui->plainTextEdit_2->appendPlainText(msg);

            ui->plainTextEdit->clear();
        }
    }

}

void MainWindow::addMessage(char * sender, char * msg, char * t) {
    QString str("\n");
    str.append(sender);
    str.append("  ");
    str.append(t);
    str.append("\n");
    str.append(msg);
    str.append("\n");

    dialogue[sender] += str;
}
void MainWindow::addMessage(QString sender, QString msg, char * t) {
    QString str("\n");
    str.append(sender);
    str.append("  ");
    str.append(t);
    str.append("\n");
    str.append(msg);
    str.append("\n");
    dialogue[sender.toStdString()] += str;

}

void MainWindow::appendText(QString text) {
    ui->plainTextEdit_2->appendPlainText(text);
}

ReadThread::ReadThread(MainWindow * obj): win(obj) {

}
void ReadThread::run() {
    getFriendsList();
    getUnreceiveMessage();

    int row_count =  win->ui->tableWidget->rowCount();
    for (int i = 0; i < row_count; i++) {
        QTableWidgetItem * item1 = win->ui->tableWidget->item(i, 0);
        QTableWidgetItem * item2 = win->ui->tableWidget->item(i, 1);
        int cnt = win->count[item1->text().toStdString()];
        if (cnt != 0) {
            item2->setText(QString::number(cnt));
        }
    }
    //循环等待接收服务器端发来的消息，此时服务端不再保存别的用户发来的消息
    SendContent content;
    while (::read(Utils::getInstance()->sockedfd, &content, sizeof(content)) != 0) {
        QString str("\n");
        str.append(QString::fromUtf8(content.sender));
        str.append("  ");
        str.append(content.sendTime);
        str.append("\n");
        str.append(QString::fromUtf8(content.message));

        //不是正在聊天的人发来消息
        //先保存到相应的数据结构中，然后显示未读消息数量
        if (content.sender != win->ui->chatingFriend->text()) {
            win->dialogue[content.sender].append(str);
            win->dialogue[content.sender].append("\n");
            win->count[content.sender]++;

            int row_count =  win->ui->tableWidget->rowCount();
            for (int i = 0; i < row_count; i++) {
                QTableWidgetItem * item1 = win->ui->tableWidget->item(i, 0);
                QTableWidgetItem * item2 = win->ui->tableWidget->item(i, 1);
                if (item1->text() == content.sender) {
                    item2->setText(QString::number(win->count[content.sender]));
                    break;
                }
            }
        } else {
            //直接在聊天窗口显示内容
            emit sendText(str);
        }
    }

}

//获取好友列表
void ReadThread::getFriendsList() {
    friends f;
    if (read(Utils::getInstance()->sockedfd, &f, sizeof(f)) == 0) {
        return;
    }
    //循环读取服务器端发来的好友列表，直到接收到结束标志
    while (f.flag != END_FLAG) {
        int rowcount = win->ui->tableWidget->rowCount();
        win->ui->tableWidget->insertRow(rowcount);
        QTableWidgetItem * item = new QTableWidgetItem(f.friendid);
        win->ui->tableWidget->setItem(rowcount, 0, item);
        QTableWidgetItem * item2 = new QTableWidgetItem("");
        win->ui->tableWidget->setItem(rowcount, 1, item2);

        //在初始化好友列表的时候顺便把相应的数据结构也初始化了
        win->dialogue.insert(pair<string, QString>(f.friendid, ""));
        win->count.insert(pair<string, int>(f.friendid, 0));

        if (read(Utils::getInstance()->sockedfd, &f, sizeof(f)) == 0) {
            return;
        }
    }
}
//接收离线时好友发来的消息
void ReadThread::getUnreceiveMessage() {
    SendContent content;
    if (read(Utils::getInstance()->sockedfd, &content, sizeof(content)) == 0) {
        return;
    }
    //循环接收直到收到结束标志
    while (content.type != END_FLAG) {
        win->addMessage(QString::fromUtf8(content.sender), QString::fromUtf8(content.message), content.sendTime);
        win->count[content.sender]++;
        if (read(Utils::getInstance()->sockedfd, &content, sizeof(content)) == 0) {
            return;
        }
    }
}






