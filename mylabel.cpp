#include "mylabel.h"

MyLabel::MyLabel(const QString & text,QWidget* parent)
    :QLabel(parent)
{
     this->setText(text);
     connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

void MyLabel::mousePressEvent(QMouseEvent *event) {
    emit clicked();
}
void MyLabel::slotClicked() {
    qDebug("hello");
}
