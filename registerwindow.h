#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>

namespace Ui {
class Registerwindow;
}

class Registerwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Registerwindow(QWidget *parent = 0);
    ~Registerwindow();

private slots:
    void on_registerButton_clicked();

    void on_returnButton_clicked();

private:
    static void * sendRegisterMsg(void *);
    static void * getRegisterMsg(void *);

private:
    static int socketfd;

private:
    Ui::Registerwindow *ui;
};

#endif // REGISTERWINDOW_H
