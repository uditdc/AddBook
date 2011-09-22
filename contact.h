#ifndef CONTACT_H
#define CONTACT_H
#include "QString"
#include "QList"
#include <QFileInfo>
class Contact
{
    public:
        Contact(QString name);
        Contact(int size, QString array[],QFileInfo contactImage);
        bool operator ==(const Contact &scnd);
        void setInfo(int size, QString array[],QFileInfo contactImage);
        QString prototypeContactString();
        void printContact();
        QList<QString> toArray();
        QString getName();
        QFileInfo getImage();
        bool isNull();
        static QString genImageId();
   private:
        // All contact variables
        QString contactId;
        QString name;
        QString age;
        QString occupation;
        QString mobile;
        QString work;
        QString address;
        QString email;
        QString comments;
        QFileInfo image;
};

#endif // CONTACT_H

