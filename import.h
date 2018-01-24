#ifndef IMPORT_H
#define IMPORT_H

#include <QDialog>
#include <QMessageBox>
namespace Ui {
class Import;
}

class Import : public QDialog
{
    Q_OBJECT

public:
    explicit Import(QWidget *parent = 0);
    ~Import();
    QString path;
    bool success = false;

private slots:
    void on_pushButton_clicked();

private:
    Ui::Import *ui;
};

#endif // IMPORT_H
