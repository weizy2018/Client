#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QLabel>
#include <map>
#include <QThread>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow;
class ReadThread;

//int getFriendsList(MainWindow *);
//int getUnreceiveMessage(MainWindow *);
//void * getServerMsg(void *);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tableWidget_clicked(const QModelIndex &index);

    void on_pushButton_clicked();
    void appendText(QString text);

public:
    //friend int getFriendsList(MainWindow *);
    //friend int getUnreceiveMessage(MainWindow *);
    //friend void * getServerMsg(void *);

public:
    void addMessage(char * sender, char * msg, char * t);
    void addMessage(QString sender, QString msg, char * t);
    //void append(const QString text);

public:
    friend class ReadThread;

private:
    Ui::MainWindow *ui;

    //QLabel * statusBarLabel;

private:
    map<string, QString> dialogue;   //<userid, message>
    map<string, int> count;

    ReadThread * thread;
};

class ReadThread: public QThread {
    Q_OBJECT
public:
    ReadThread(MainWindow * obj);

signals:
    void sendText(QString msg);

protected:
    void run();

private:
    void getFriendsList();
    void getUnreceiveMessage();

private:
    MainWindow * win;

};

#endif // MAINWINDOW_H
