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
        ui->state->setText(QString::fromUtf8("登录失败！！！"));
        return;
    }

    User u;
    QByteArray array;
    QString account = ui->account->text();
    QString password = ui->password->text();

    array = account.toLatin1();
    memcpy(u.account, array.data(), account.length());

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


    pthread_join(send_thread, NULL);
    pthread_join(rec_thread, NULL);

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

}
