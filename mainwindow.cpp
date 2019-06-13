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

    //statusBarLabel = new QLabel("");
    //ui->statusBar->addWidget(statusBarLabel);

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
    QTableWidgetItem * item1 = ui->tableWidget->item(index.row(), 0);
    QTableWidgetItem * item2 = ui->tableWidget->item(index.row(), 1);

    if (item1->text() != ui->chatingFriend->text() && ui->chatingFriend->text() != "") {
        dialogue[ui->chatingFriend->text().toStdString()] = ui->plainTextEdit_2->toPlainText();
    }

    if (item1->text() != ui->chatingFriend->text()) {
        ui->chatingFriend->setText(item1->text());
        qDebug() << item1->text();


        ui->plainTextEdit_2->setPlainText(dialogue[item1->text().toStdString()]);

        //将光标放置文本末尾
        /*
        QTextCursor cursor = ui->plainTextEdit_2->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->plainTextEdit_2->setTextCursor(cursor);*/

        //ui->plainTextEdit_2->moveCursor(QTextCursor::End);
        /*

        QScrollBar *scrollbar = ui.taskStatusTextEdit->verticalScrollBar();
        11     if (scrollbar)
        12     {
        13         scrollbar->setSliderPosition(scrollbar->maximum());
        14     }
          */
        /*
        QScrollBar * sc = ui->plainTextEdit_2->verticalScrollBar();
        if (sc) {
            sc->setSliderPosition(sc->maximum());

        }
        */
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
            ui->plainTextEdit_2->appendPlainText(QString::fromUtf8("我  ") + QString::fromStdString(currentTime));
            //ui->plainTextEdit_2->appendPlainText(QString::fromStdString(currentTime));
            ui->plainTextEdit_2->appendPlainText(msg);
            ui->plainTextEdit_2->appendPlainText("");

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

        //在初始化好友列表的时候顺便把相应的数据结构也初始化了
        window->dialogue.insert(pair<string, QString>(f.friendid, ""));
        window->count.insert(pair<string, int>(f.friendid, 0));

        if (read(Utils::getInstance()->sockedfd, &f, sizeof(f)) == 0) {
            return -1;
        }
    }
    return 0;
}

int getUnreceiveMessage(MainWindow * window) {

    SendContent content;
    if (read(Utils::getInstance()->sockedfd, &content, sizeof(content)) == 0) {
        return -1;
    }
    while (content.type != END_FLAG) {
        window->addMessage(content.sender, content.message, content.sendTime);
        window->count[content.sender]++;
        if (read(Utils::getInstance()->sockedfd, &content, sizeof(content)) == 0) {
            return -1;
        }
    }

    return 0;
}

//接受服务器端发来的消息
void * getServerMsg(void * w) {
    MainWindow * window = (MainWindow*)w;
    getFriendsList(window);
    getUnreceiveMessage(window);
    /*
    qDebug("==================");
    for (auto it = window->dialogue.begin(); it != window->dialogue.end(); it++) {
        qDebug() << QString::fromStdString(it->first) << " " << it->second;
    }*/

    int row_count =  window->ui->tableWidget->rowCount();
    for (int i = 0; i < row_count; i++) {
        QTableWidgetItem * item1 = window->ui->tableWidget->item(i, 0);
        QTableWidgetItem * item2 = window->ui->tableWidget->item(i, 1);
        int cnt = window->count[item1->text().toStdString()];
        if (cnt != 0) {
            item2->setText(QString::number(cnt));
        }
    }
    //监听在线时接收到的消息，此时服务端不再保存别的用户发来的消息
    //...
    SendContent content;
    while (read(Utils::getInstance()->sockedfd, &content, sizeof(content)) != 0) {
        QString str("\n");
        str.append(content.sender);
        str.append("  ");
        str.append(content.sendTime);
        str.append("\n");
        str.append(content.message);

        //不是正在聊天的人发来消息
        //先保存到相应的数据结构中，然后显示未读消息数量
        if (content.sender != window->ui->chatingFriend->text()) {
            window->dialogue[content.sender].append(str);
            window->count[content.sender]++;

            int row_count =  window->ui->tableWidget->rowCount();
            for (int i = 0; i < row_count; i++) {
                QTableWidgetItem * item1 = window->ui->tableWidget->item(i, 0);
                QTableWidgetItem * item2 = window->ui->tableWidget->item(i, 1);
                if (item1->text() == content.sender) {
                    item2->setText(QString::number(window->count[content.sender]));
                    break;
                }
            }
        } else {
            //直接显示出来
            window->ui->plainTextEdit_2->appendPlainText(str);
        }
    }
    return NULL;
}







