#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "loginwindow.h"
#include "utils.h"

#include <qdebug.h>
#include <QMessageBox>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

int Registerwindow::socketfd = 0;
Registerwindow::Registerwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registerwindow)
{
    ui->setupUi(this);
}

Registerwindow::~Registerwindow()
{
    delete ui;
}

void Registerwindow::on_registerButton_clicked()
{
    struct sockaddr_in address;
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    address.sin_port = htons(PORT);

    //连接服务端
    int result = ::connect(socketfd, (struct sockaddr *)&address, sizeof(address));
    //无法连接时显示出错信息
    if (result == -1) {
        qDebug("connect fail!!!");
        ui->state->setText(QString::fromUtf8("网络连接错误！！！"));
        return;
    }

    User u;
    QByteArray array;
    QString account = ui->accountEdit->text();
    QString passwd = ui->passwdEdit->text();
    qDebug() << "account: " << account;
    qDebug() << "passwd: " << passwd;

    array = account.toLatin1();
    strcpy(u.account, array.data());

    array = passwd.toLatin1();
    strcpy(u.password, array.data());

    pthread_t send_thread, rec_thread;
    if ((::pthread_create(&send_thread, NULL, sendRegisterMsg, &u)) != 0) {
        ui->state->setText(QString::fromUtf8("注册失败！！！"));
        return;
    }
    if ((::pthread_create(&rec_thread, NULL, getRegisterMsg, NULL)) != 0) {
        ui->state->setText(QString::fromUtf8("注册失败！！！"));
        return;
    }
    void * msg;
    pthread_join(send_thread, NULL);
    pthread_join(rec_thread, &msg);

    char * ch = (char*)msg;

    //注册成功
    if (strcmp(ch, REGISTER_SUCCESS) == 0) {
        QMessageBox::information(this, "", QString::fromUtf8("注册成功"), QMessageBox::Yes, QMessageBox::Yes);
        LoginWindow * lw = new LoginWindow();
        lw->show();
        delete this;
    } else {
        ui->state->setText(QString::fromUtf8("该账号已经存在"));
    }

}

void Registerwindow::on_returnButton_clicked()
{
    LoginWindow * lw = new LoginWindow();
    lw->show();
    delete this;
}

void * Registerwindow::sendRegisterMsg(void *user) {
    User u = *(User*)user;

    SendContent content;
    memset(&content, 0, sizeof(content));

    content.type = REGISTER_ACTION;
    strcpy(content.receiver, u.password);
    strcpy(content.sender, u.account);

    write(socketfd, &content, sizeof(content));
    return NULL;
}

void * Registerwindow::getRegisterMsg(void *) {
    char * result = new char[10];
    read(socketfd, result, 10);
    return result;
}
