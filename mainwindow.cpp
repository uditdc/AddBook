#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "searchfilter.h"
#include "contact.h"
#include "QLinkedList"
#include "QCompleter"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QtCore>
#include <QtSql>

using namespace std;

// Declare the contacts lists
QList<Contact> contactList;
QLineEdit *fields[8];
QFileInfo ContactImage;
QSqlDatabase db;
static int TOTAL_ATTR = 8;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SearchFilter *filtS = new SearchFilter();
    ui->search->installEventFilter(filtS);
    initializeFields();

    if(!createDbConnection()) QMessageBox::critical(this,tr("Address Book"),
                                tr("The Databaase connection could not be established"));
    initDb();
    getEntry();
        removeUnusedImages();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeFields(){

    fields[0] = ui->txt_name;
    fields[1] = ui->txt_age;
    fields[2] = ui->txt_occupation;
    fields[3] = ui->txt_mobile;
    fields[4] = ui->txt_work;
    fields[5] = ui->txt_addr;
    fields[6] = ui->txt_email;
    fields[7] = ui->txt_comments;



    searchText("");
}

void MainWindow::newEntry(){
    clearFields();
    ui->txt_name->setFocus();
}

void MainWindow::saveEntry(){
    // Declaration
    QString array[8];

    for(int i=0;i<(sizeof(fields)/sizeof(fields[0]));i++){
        array[i] = fields[i]->text();
    }

    if(!array[0].isEmpty()){

        QSqlQuery query;
        query.prepare("insert into contact (name, age, occupation, mobile, work,address, email,comments, imageId)"
                      "values(:name,:age,:occupation,:mobile,:work,:address,:email,:comments,:imageId)");
        int sizeOfArry = sizeof(array)/sizeof(QString);
        for(int i=0;i<sizeOfArry;i++){
            query.bindValue(i,array[i]);
        }

        query.bindValue(sizeOfArry,ContactImage.filePath());

        bool stat = query.exec();

        if(stat){

            contactList.append(Contact((sizeof(array)/sizeof(QString)),array,ContactImage));
        }else{
           int action =  QMessageBox::warning(this,tr("Address Book"),
                                 tr("A Contact with the same name exists, do you want to update the information."),
                                 QMessageBox::Yes|QMessageBox::No);
           switch(action){
               case QMessageBox::Yes : editContact(getContact(array[0]),array,ContactImage);
               break;
           }
        }
    }

    clearFields();
    QString srchTxt = ui->search->text();
    if(srchTxt == "Search"){
        searchText("");
    }else searchText(srchTxt);
}

void MainWindow::deleteEntry(){
    if(ui->lst_search->selectedItems().isEmpty()){
        QMessageBox::warning(this,tr("Address Book"),tr("No Contact is Selected to Delete."));
    }else{
        Contact contact =  getContact(ui->lst_search->currentItem()->text());
        QSqlQuery query;
        query.prepare("Select imageId from contact where name = :name");
        query.addBindValue(contact.getName());
        query.exec();

        while(query.next()){
            QString imageId = query.value(0).toString();
            if(!imageId.isEmpty()) QFile::remove(imageId);
        }

        query.clear();
        query.prepare("delete from contact where name = :name");
        query.addBindValue(contact.getName());
        query.exec();

        contactList.remove(contact);
        clearFields();
        searchText("");
    }

}

void MainWindow::editContact(Contact contact, QString array[], QFileInfo contactImage){

    QSqlQuery query;
    query.prepare("update contact set age = ?, occupation = ?, mobile = ?, work = ?,"
                  "address = ?, email = ?, comments = ?, imageId = ? "
                  "where name = '"+contact.getName()+"';");
    for(int i = 1;i<TOTAL_ATTR;i++){
        query.addBindValue(array[i]);
    }
    query.addBindValue(contactImage.filePath());
    query.exec();
    contact.setInfo(TOTAL_ATTR,array,contactImage);
    contactList.replace(contactList.indexOf(contact),contact);
}

void MainWindow::clearFields(){

    for(int i=0;i<(sizeof(fields)/sizeof(fields[0]));i++){
        fields[i]->setText("");
    }
    ContactImage.setFile("");
    ui->btn_image->setIcon(QIcon(":/rsc/face.png"));

}

void MainWindow::displayContact(Contact contact){
    QList<QString> array(contact.toArray());

    for(int i=0;i<(sizeof(fields)/sizeof(fields[0]));i++){
        fields[i]->setText(array.at(i));
    }
    if(!contact.getImage().filePath().isEmpty()){
        ContactImage = contact.getImage();
        ui->btn_image->setIcon(QIcon(contact.getImage().filePath()));
    }else ui->btn_image->setIcon(QIcon(":/rsc/face.png"));
}

void MainWindow::searchText(QString text){

    if(text!="Search"){
        ui->lst_search->clear();
        foreach(Contact c, contactList){
            QString name = c.getName();
            if(name.toLower().startsWith(text.toLower())||text=="") ui->lst_search->addItem(name);
        }
    }
}

void MainWindow::contactClicked(QListWidgetItem *contact){
    if(!getContact(contact->text()).isNull()){
            displayContact(getContact(contact->text()));
    }
}

Contact MainWindow::getContact(QString name){
    foreach(Contact c,contactList){
        if(c.getName() == name) return c ;
    }
}

void MainWindow::changeImage(){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/udit", tr("Image Files (*.png *.jpg *.bmp)"));

    QFileInfo file(fileName);
    QFileInfo imageDir("image/");

    QDir dir;
    if(!dir.exists(imageDir.path())) dir.mkdir(imageDir.path());
    dir.cd(imageDir.path());
    ContactImage = imageDir.path()+"/"+Contact::genImageId();

    bool res = QFile::copy(file.absoluteFilePath(),ContactImage.filePath());
    if(!res) QMessageBox::warning(this,tr("Address Book"), tr("File cant be copied"),QMessageBox::Ok);

    ui->btn_image->setIcon(QIcon(ContactImage.filePath()));

}

bool MainWindow::createDbConnection(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("addBook.db");

    if(!db.open()) return false ;
    return true;
}

void MainWindow::initDb(){
    QSqlQuery query;
    query.exec("create table if not exists contact(name TEXT PRIMARY KEY ,age TEXT, occupation TEXT, mobile TEXT, work TEXT,address TEXT, email TEXT,comments TEXT, imageId TEXT)");

}

void MainWindow::getEntry(){
    QSqlQuery query;
    query.exec("Select * from contact");
    QString data[8];

    while(query.next()){
        data[0] = query.value(0).toString();
        data[1] = query.value(1).toString();
        data[2] = query.value(2).toString();
        data[3] = query.value(3).toString();
        data[4] = query.value(4).toString();
        data[5] = query.value(5).toString();
        data[6] = query.value(6).toString();
        data[7] = query.value(7).toString();

        if(!data[0].isEmpty()) contactList.append(Contact((sizeof(data)/sizeof(QString)),data,QFileInfo(query.value(8).toString())));

    }
}

void MainWindow::removeUnusedImages(){
    QSqlQuery query;
    query.exec("Select imageId from contact");
    QList<QString> exstImages;

    while(query.next()){

        exstImages << query.value(0).toString();
    }
    QDir baseDir("image");
    baseDir.setFilter(QDir::Files);
    QStringList lst = baseDir.entryList();

    foreach (QString file, lst) {
        if(!exstImages.contains(file)) QFile::remove(baseDir.path() +"/"+file);
    }
}
