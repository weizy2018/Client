#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>



LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void * sendLoginMessage(void *);
void * getLoginMessage(void *);

void LoginWindow::on_loginButton_clicked()
{
    struct sockaddr_in address;
    Utils::getInstance()->sockedfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    address.sin_port = htons(PORT);

    int result = ::connect(Utils::getInstance()->sockedfd, (struct sockaddr *)&address, sizeof(address));
    if (result == -1) {
        qDebug("connect fail!!!");
        ui->state->setText(QString::fromUtf8("网络连接错误！！！"));
        return;
    }

    User u;
    QByteArray array;
    QString account = ui->account->text();
    QString password = ui->password->text();

    array = account.toLatin1();
    strcpy(u.account, array.data());

    array = password.toLatin1();
    strcpy(u.password, array.data());

    pthread_t send_thread, rec_thread;
    if ((::pthread_create(&send_thread, NULL, sendLoginMessage, &u)) != 0) {
        ui->state->setText(QString::fromUtf8("登录失败！！！"));
        return;
    }
    if ((::pthread_create(&rec_thread, NULL, getLoginMessage, NULL)) != 0) {
        ui->state->setText(QString::fromUtf8("登录失败！！！"));
        return;
    }

    void * loginFlag;
    pthread_join(send_thread, NULL);
    pthread_join(rec_thread, &loginFlag);

    char * ch = (char*)loginFlag;
    qDebug(ch);

    if (strcmp(ch, LOGIN_SUCCESS) != 0) {
        ui->state->setText(QString::fromUtf8("账号或密码错误！！！"));
        return;
    }

    MainWindow * m = new MainWindow;
    m->show();
    this->close();
}

void * sendLoginMessage(void * user)
{
    User u = *(User*)user;

    SendContext content;
    memset(&content, 0, sizeof(content));

    content.type = LOGIN_ACTION;
    strcpy(content.receiver, u.password);
    strcpy(content.sender, u.account);

    write(Utils::getInstance()->sockedfd, &content, sizeof(content));
    return NULL;
}
void * getLoginMessage(void *) {
    char * result = new char[10];
    read(Utils::getInstance()->sockedfd, result, 10);
    return result;
}

void LoginWindow::on_registerLabel_linkActivated(const QString &link)
{

}
