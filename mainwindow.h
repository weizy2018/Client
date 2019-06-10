#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
class MainWindow;
}

class MainWindow;

int getFriendsList(MainWindow *);
void * getServerMsg(void *);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tableWidget_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

public:
    friend int getFriendsList(MainWindow *);
    friend void * getServerMsg(void *);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
