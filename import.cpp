#include "import.h"
#include "ui_import.h"
#include <phonebook.h>

Import::Import(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Import)
{
    ui->setupUi(this);
    success = false;
}

Import::~Import()
{
    delete ui;
}

void Import::on_pushButton_clicked()
{
    path = ui->txtRes->toPlainText();
    success = true;
    close();
}
