#include "settingwindow.h"
#include "ui_settingwindow.h"

SettingWindow::SettingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::on_mainPage_clicked()
{
    this->parentWidget()->show();
    delete this;
}
