#ifndef CREATE_H
#define CREATE_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class Create;
}

class Create : public QDialog
{
    Q_OBJECT

public:
    explicit Create(QWidget *parent = 0);
    ~Create();
    QString fio;
    QString phone;
    QString email;
    QString birthday;
    QString prepath;
    QString path;
    bool success;
private slots:

    void on_create_clicked();

    void on_cancel_clicked();

    void on_pushButton_clicked();

private:
    Ui::Create *ui;

};

#endif // CREATE_H
