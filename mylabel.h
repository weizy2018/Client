#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QWidget>
#include <QString>
#include <QMouseEvent>

class MyLabel: public QLabel
{
    Q_OBJECT
public:
    MyLabel(const QString &text,QWidget *parent=0);
    ~MyLabel(){}
signals:
    void clicked();
public slots:
    void slotClicked();
protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // MYLABEL_H
