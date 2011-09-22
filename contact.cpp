#include "contact.h"
#include "QString"
#include "QStringBuilder"
#include "QList"
#include "string"
#include "iostream"
#include <QTime>
#include "stdlib.h"
#include "time.h"
using namespace std;

static int TOTAL_ATTR = 8;

Contact::Contact(QString name)
{
    this->name =name;
}


Contact::Contact(int size, QString array[],QFileInfo contactImage){
    setInfo(size,array,contactImage);
}

bool Contact::operator ==(const Contact &scnd){
    return (this->name == scnd.name);
}

void Contact::setInfo(int size, QString array[],QFileInfo contactImage){
    if(size == TOTAL_ATTR){
        name = array[0]; age = array[1]; occupation = array[2] ; mobile = array[3];
        work = array[4] ; address = array[5]; email = array[6] ; comments = array[7];
    }
    image = contactImage;
}

QList<QString> Contact::toArray(){
    QList<QString> array;

    array << name << age << occupation << mobile << work << address << email << comments;
    return array;
}

QString Contact::prototypeContactString(){
    QString cString;
    cString.append("\n Name : ");
    cString.append(name);
    cString.append("\n Age : ");
    cString.append(age);
    cString.append("\n Occupation : ");
    cString.append(occupation);
    cString.append("\n Mobile : ");
    cString.append(mobile);
    cString.append("\n Work : ");
    cString.append(work);
    cString.append("\n Address : ");
    cString.append(address);
    cString.append("\n E-Mail : ");
    cString.append(email);
    cString.append("\n Comment : ");
    cString.append(comments);

    return cString;
}

void Contact::printContact(){
    cout << prototypeContactString().toStdString() << endl;
}

QString Contact::getName(){
    return name;
}

QFileInfo Contact::getImage(){
    return image;
}

bool Contact::isNull(){
    if(name.isNull()){
        return true;
    }else return false;
}

QString Contact::genImageId(){

    srand(time(NULL));

    return "img_"+QString::number((rand() % 10000)+1);
}
