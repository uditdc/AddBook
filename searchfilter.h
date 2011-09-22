#ifndef SEARCHFILTER_H
#define SEARCHFILTER_H

#include <QObject>

class SearchFilter : public QObject
{
    Q_OBJECT
public:
    explicit SearchFilter(QObject *parent = 0);
protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:

public slots:

};

#endif // SEARCHFILTER_H
