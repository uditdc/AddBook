#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "contact.h"
#include "QListWidget"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void displayContact(Contact contact);
    Contact getContact(QString name);
    void reloadList();
    bool createDbConnection();
    void initDb();
    void getEntry();
    void editContact(Contact contact, QString array[], QFileInfo contactImage);
    void removeUnusedImages();
private:
    void clearFields();
    void initializeFields();

public slots:
    void saveEntry();
    void searchText(QString text);
    void contactClicked(QListWidgetItem*);
    void newEntry();
    void changeImage();
    void deleteEntry();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
