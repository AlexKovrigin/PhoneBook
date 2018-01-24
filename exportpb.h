#ifndef EXPORTPB_H
#define EXPORTPB_H

#include <QDialog>

namespace Ui {
class exportPB;
}

class exportPB : public QDialog
{
    Q_OBJECT

public:
    explicit exportPB(QWidget *parent = 0);
    ~exportPB();
    QString path;
    bool success;

private slots:
    void on_pushButton_clicked();

private:
    Ui::exportPB *ui;
};

#endif // EXPORTPB_H
