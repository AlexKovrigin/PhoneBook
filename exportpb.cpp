#include "exportpb.h"
#include "ui_exportpb.h"

exportPB::exportPB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exportPB)
{
    ui->setupUi(this);
    success = false;
}

exportPB::~exportPB()
{
    delete ui;
}

void exportPB::on_pushButton_clicked()
{
    path = ui->textBrowser->toPlainText();
    success = true;
    close();
}
