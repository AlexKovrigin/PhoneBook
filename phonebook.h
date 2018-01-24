#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <QMainWindow>
#include <create.h>
#include <QMessageBox>
#include <import.h>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <exportpb.h>
#include <about.h>
#include <QFileDialog>
#include <QVector>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QDebug>
namespace Ui {
class PhoneBook;
}

class PixmapLabel;
class FString;

class PhoneBook : public QMainWindow
{
    Q_OBJECT

public:
    explicit PhoneBook(QWidget *parent = 0);
    ~PhoneBook();

    void importPB();
    QString path;


private slots:
    void on_btnAdd_clicked();

    void on_btnDel_clicked();

    void on_txtSearch_textChanged();

    void on_actionImport_triggered();

    void on_actionExport_triggered();

    void on_actionAbout_us_triggered();

    void on_btnDelAll_clicked();

    void on_pushButton_clicked();

    void on_tablePhones_clicked(const QModelIndex &index);

private:
    Ui::PhoneBook *ui;
    QVector <QString> photoarray;
    PixmapLabel* lbl;
    QPixmap pix;

};

#endif // PHONEBOOK_H
