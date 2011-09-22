#include "searchfilter.h"
#include "QEvent"
#include <QLineEdit>
SearchFilter::SearchFilter(QObject *parent) :
    QObject(parent)
{
}

bool SearchFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::FocusIn) {
        QLineEdit *comp = qobject_cast<QLineEdit *>(obj);
        QString text = comp->text();
        if(text=="Search") comp->setText("");
        return false;
    }else if(event->type() == QEvent::FocusOut){
        QLineEdit *comp = qobject_cast<QLineEdit *>(obj);
        QString text = comp->text();
        if(text=="") comp->setText("Search");
        return false;
    }else {
        return QObject::eventFilter(obj, event);
    }
}
