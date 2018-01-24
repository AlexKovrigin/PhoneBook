#include "create.h"
#include "ui_create.h"

Create::Create(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Create)
{
    ui->setupUi(this);
    success = false;
    QDate date = QDate::currentDate();
    ui->dateEdit->setMaximumDate(date);
}

Create::~Create()
{
    delete ui;

}

void Create::on_create_clicked()
{
    fio = ui->txtFIO->toPlainText();
    phone = ui->txtPhone->toPlainText();
    email = ui->txtEmail->toPlainText();
    birthday = ui->dateEdit->date().toString("dd/MM/yyyy");
    path = prepath;
    success = true;
    close();
}

void Create::on_cancel_clicked()
{
    close();
}

void Create::on_pushButton_clicked()
{
    prepath = QFileDialog::getOpenFileName(this, tr("Open File"),"C://", "All Files(*);;Image File (*.jpg);;Image File (*.png)");
    ui->lblphoto->setText(prepath);
}
