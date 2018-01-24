#include "phonebook.h"
#include "ui_phonebook.h"
#include <QMessageBox>
#include <create.h>

class PixmapLabel : public QLabel{
public:
    explicit PixmapLabel(QWidget* parent=nullptr):QLabel(parent){
        //By default, this class scales the pixmap according to the label's size
        setScaledContents(true);
    }
    ~PixmapLabel(){}

protected:
    void paintEvent(QPaintEvent* event);
private:
    //QImage to cache the pixmap()
    //to avoid constructing a new QImage on every scale operation
    QImage cachedImage;
    //used to cache the last scaled pixmap
    //to avoid calling scale again when the size is still at the same
    QPixmap scaledPixmap;
    //key for the currently cached QImage and QPixmap
    //used to make sure the label was not set to another QPixmap
    qint64 cacheKey{0};
};

//based on the implementation of QLabel::paintEvent
void PixmapLabel::paintEvent(QPaintEvent *event){
    //if this is assigned to a pixmap
    if(pixmap() && !pixmap()->isNull()){
        QStyle* style= PixmapLabel::style();
        QPainter painter(this);
        drawFrame(&painter);
        QRect cr = contentsRect();
        cr.adjust(margin(), margin(), -margin(), -margin());
        int align= QStyle::visualAlignment(layoutDirection(), alignment());
        QPixmap pix;
        if(hasScaledContents()){ //if scaling is enabled
            QSize scaledSize= cr.size() * devicePixelRatioF();
            //if scaledPixmap is invalid
            if(scaledPixmap.isNull() || scaledPixmap.size()!=scaledSize
                    || pixmap()->cacheKey()!=cacheKey){
                //if cachedImage is also invalid
                if(pixmap()->cacheKey() != cacheKey){
                    //reconstruct cachedImage
                    cachedImage= pixmap()->toImage();
                }
                QImage scaledImage= cachedImage.scaled(
                            scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                scaledPixmap= QPixmap::fromImage(scaledImage);
                scaledPixmap.setDevicePixelRatio(devicePixelRatioF());
            }
            pix= scaledPixmap;
        } else { // no scaling, Just use pixmap()
            pix= *pixmap();
        }
        QStyleOption opt;
        opt.initFrom(this);
        if(!isEnabled())
            pix= style->generatedIconPixmap(QIcon::Disabled, pix, &opt);
        style->drawItemPixmap(&painter, cr, align, pix);
    } else { //otherwise (if the label is not assigned to a pixmap)
        //call base paintEvent
        QLabel::paintEvent(event);
    }
}





/*/////////////////////////////////////////////////////////////////////////////////////////
                                            ^
                                            |
                       The Border of boring code and code made by Шурик


/////////////////////////////////////////////////////////////////////////////////////////*/


PhoneBook::PhoneBook(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PhoneBook)
{
    ui->setupUi(this);
    //TODO default.txt, авто-загрузка при запуске программы и авто-сохранение при выходе
    QFile mFile("default.txt");
    QString name = qgetenv("USERNAME");
    if(name.isEmpty())
        name = qgetenv("USER");
    if(!mFile.open(QFile::ReadOnly)/* || name == "salex"*/)
    {
        QMessageBox msgBox;
        msgBox.setText("Welcome to my amazing app, " + name + "! :D");
        msgBox.exec();
    }
    QTextStream in(&mFile);
    int rowAmount = ui->tablePhones->rowCount();
    for(int o = 0; o < rowAmount; o++)
        ui->tablePhones->removeRow(0);
    while(!in.atEnd())
    {
        QString line = in.readLine(1000);
        int row = ui->tablePhones->rowCount() - 1;
        //int row = -1;
        ui->tablePhones->insertRow(row + 1);
        int i = 0;
        int prevInt = 0;
        for(; i < 1000; i++)
        {
            if(line[i] == 0)
            {
                QString name;
                for(int a = 0; a < i; a++)
                    name += line[a];
                ui->tablePhones->setItem(row + 1, 0, new QTableWidgetItem(name));
                prevInt = ++i;
                break;
            }
        }
        for(; i < 1000; i++)
        {
            if(line[i] == 0)
            {
                QString phone;
                for(int a = prevInt; a < i; a++)
                    phone += line[a];
                ui->tablePhones->setItem(row + 1, 1, new QTableWidgetItem(phone));
                prevInt = ++i;
                break;
            }
        }
        for(; i < 1000; i++)
        {
            if(line[i] == 0)
            {
                QString email;
                for(int a = prevInt; a < i; a++)
                    email += line[a];
                ui->tablePhones->setItem(row + 1, 2, new QTableWidgetItem(email));
                prevInt = ++i;
                break;
            }
        }
        for(; i < 1000; i++)
        {
            if(line[i] == 0)
            {
                QString birthday;
                for(int a = prevInt; a < i; a++)
                    birthday += line[a];
                ui->tablePhones->setItem(row + 1, 3, new QTableWidgetItem(birthday));
                prevInt = ++i;
                break;
            }
        }
        for(; i < 1000; i++)
        {
            if(line[i] == 0)
            {
                QString path;
                for(int a = prevInt; a < i; a++)
                    path += line[a];
                photoarray.push_back(path);
                prevInt = ++i;
                break;
            }
        }
    }
    if(ui->tablePhones->rowCount()) {
        pix.load(photoarray[0]);
        ui->label_2->setPixmap(pix);
        ui->label_2->setBackgroundRole(QPalette::Base); // Specialy made errors
        ui->label_2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui->label_2->setScaledContents(true);
    }
    ui->btnDel->setEnabled(true);
    if(!ui->tablePhones->rowCount())
        ui->btnDel->setEnabled(false);
    QString date = QDate::currentDate().toString("dd/MM");
    for(int i = 0; i < ui->tablePhones->rowCount(); i++) {
        QString newdate = ui->tablePhones->item(i, 3)->text();
        newdate = newdate.left(5);
        if(date == newdate)
        {
            QString yearOfBirth = ui->tablePhones->item(i, 3)->text();
            yearOfBirth = yearOfBirth.right(4);
            int yearB = yearOfBirth.toInt();
            int yearNow = QDate::currentDate().toString("yyyy").toInt();
            int age = yearNow - yearB;
            QMessageBox msgBox;
            msgBox.setText(ui->tablePhones->item(i, 0)->text() + " сегодня празднует свой день рождения! Ему(ей) исполнилось " + QString::number(age));
            msgBox.exec();
        }
    }
    return;
}

PhoneBook::~PhoneBook()
{
    QFile file("default.txt");
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Sorry, the path, that you gave is incorrect!");
        msgBox.exec();
    }

    QTextStream out(&file);
    QChar nul = 0;
    for(int i = 0; i < ui->tablePhones->rowCount(); i++)
    {
        out << ui->tablePhones->item(i, 0)->text() << nul;
        out << ui->tablePhones->item(i, 1)->text() << nul;
        out << ui->tablePhones->item(i, 2)->text() << nul;
        out << ui->tablePhones->item(i, 3)->text() << nul;
        out << photoarray[i] << nul << endl;
        file.flush();
    }

    file.close();
    delete ui;
}

void PhoneBook::importPB()
{
    /*Import y;
    y.exec();
    if(!y.success)
        return;*/
    // TODO проверить работу программы на 1000000 элементах.
    path = QFileDialog::getOpenFileName(this, tr("Open File"),"C://", "All Files(*);;Text File (*.txt)");
    QFile mFile(path);
    if(!mFile.open(QFile::ReadOnly))
        return;
    QTextStream in(&mFile);
    int rowAmount = ui->tablePhones->rowCount();
    for(int o = 0; o < rowAmount; o++)
        ui->tablePhones->removeRow(0);
    while(!in.atEnd())
    {
        QString line = in.readLine(1000);
        int row = ui->tablePhones->currentRow();
        ui->tablePhones->insertRow(row + 1);
        int i = 0;
        int prevInt = 0;
        for(; i < 1000; i++)
        {
            if(line[i] == 0)
            {
                QString name;
                for(int a = 0; a < i; a++)
                    name += line[a];
                ui->tablePhones->setItem(row + 1, 0, new QTableWidgetItem(name));
                prevInt = ++i;
                break;
            }
        }
        for(; i < 1000; i++)
        {
            if(line[i] == 0)
            {
                QString phone;
                for(int a = prevInt; a < i; a++)
                    phone += line[a];
                ui->tablePhones->setItem(row + 1, 1, new QTableWidgetItem(phone));
                prevInt = ++i;
                break;
            }
        }
        for(; i < 1000; i++)
        {
            if(line[i] == 0)
            {
                QString email;
                for(int a = prevInt; a < i; a++)
                    email += line[a];
                ui->tablePhones->setItem(row + 1, 2, new QTableWidgetItem(email));
                prevInt = ++i;
                break;
            }
        }
        for(; i < 1000; i++)
        {
            if(line[i] == 0)
            {
                QString birthday;
                for(int a = prevInt; a < i; a++)
                    birthday += line[a];
                ui->tablePhones->setItem(row + 1, 3, new QTableWidgetItem(birthday));
                prevInt = ++i;
                break;
            }
        }
        for(; i < 1000; i++)
        {
            if(line[i] == 0)
            {
                QString path;
                for(int a = prevInt; a < i; a++)
                    path += line[a];
                photoarray.push_back(path);
                prevInt = ++i;
                break;
            }
        }
    }
    ui->btnDel->setEnabled(true);
    if(!ui->tablePhones->rowCount())
        ui->btnDel->setEnabled(false);
    return;
}

void PhoneBook::on_btnAdd_clicked()
{
    Create z;
    z.exec();
    if(!(z.success))
        return;
    //TODO при TAB осуществлять переход между полями ввода tab thing
    int row = ui->tablePhones->currentRow();
    ui->tablePhones->insertRow(row + 1);
    //QTableWidgetItem item("Hello");
    //ui->tablePhones->setItem(row, 0, &item);

    //QMessageBox msgBox;
    //msgBox.setText("The document has been modified.");
    //msgBox.exec();

    //QTableWidgetItem *item=new QTableWidgetItem ("TEST");
    ui->tablePhones->setItem(row + 1, 0, new QTableWidgetItem(z.fio));
    ui->tablePhones->setItem(row + 1, 1, new QTableWidgetItem(z.phone));
    ui->tablePhones->setItem(row + 1, 2, new QTableWidgetItem(z.email));
    ui->tablePhones->setItem(row + 1, 3, new QTableWidgetItem(z.birthday));
    photoarray.push_back(z.path);
    if(photoarray[photoarray.count() - 1] == "")
        photoarray[photoarray.count() - 1] = ":/man.ico";
    int i = photoarray.count() - 1;
    QString justforlolz = photoarray[i];
    ui->btnDel->setEnabled(true);
    if(!ui->tablePhones->rowCount())
        ui->btnDel->setEnabled(false);
}

void PhoneBook::on_btnDel_clicked()
{
    int row;
    if(ui->tablePhones->currentRow() == -1)
        row = ui->tablePhones->rowCount() - 1;
    else
        row = ui->tablePhones->currentRow();

    ui->tablePhones->removeRow(row);
    photoarray.erase(photoarray.begin() + row);
    //TODO удаление из photoarray и локального файла.
    if(!ui->tablePhones->rowCount())
        ui->btnDel->setEnabled(false);
}

/*void PhoneBook::on_btnSearch_clicked()
{
    QString str = "Кон";

    int foundRow = -1;
    int foundCol = -1;

    for (int i = 0; i < ui->tablePhones->rowCount(); i++)
    {
        for (int j = 0; j < ui->tablePhones->columnCount(); j++)
        {
            if (ui->tablePhones->item(i,j)->text().contains(str, Qt::CaseInsensitive))
            {
                foundRow = i;
                foundCol = j;
                break;
            }
        }
        if (foundRow >= 0) break;
    }

    if (foundRow >=0)
    {
        ui->tablePhones->selectRow(foundRow);
        //ui->tablePhones->select Column(foundCol);
        //ui->tablePhones->model()->index(foundRow,foundCol);
       // ui->tablePhones->selectionModel();

    }

}*/

void PhoneBook::on_txtSearch_textChanged()
{ // TODO пересмотреть поиск, быстрая работа
    //importPB(path);
    ui->tablePhones->clearSelection();
    QString search;
    search = ui->txtSearch->toPlainText();
    if(search == "")
        return;
    int row = ui->tablePhones->rowCount();
    //QString* l = new string[row];
    ui->tablePhones->setSelectionMode(QAbstractItemView::MultiSelection);
    for(int i = 0; i < row; i++)
    {
        for (int j = 0; j < ui->tablePhones->columnCount(); j++)
        {
            if (ui->tablePhones->item(i,j)->text().contains(search, Qt::CaseInsensitive)) {
                ui->tablePhones->selectRow(i);
                break;
            }
        }
    }
    ui->tablePhones->setSelectionMode(QAbstractItemView::SingleSelection);
}

void PhoneBook::on_actionImport_triggered()
{
    importPB();
}

void PhoneBook::on_actionExport_triggered()
{ // TODO прогресс бар при экспорте
    /*exportPB u;
    u.exec();
    if(!u.success)
        return;*/
    path = QFileDialog::getSaveFileName(this, tr("Save File"),"C://", "All Files(*);;Text File (*.txt)");
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
        return;

    QTextStream out(&file);
    QChar nul = 0;
    for(int i = 0; i < ui->tablePhones->rowCount(); i++)
    {
        out << ui->tablePhones->item(i, 0)->text() << nul;
        out << ui->tablePhones->item(i, 1)->text() << nul;
        out << ui->tablePhones->item(i, 2)->text() << nul;
        out << ui->tablePhones->item(i, 3)->text() << nul;
        out << photoarray[i] << nul << endl;
    }
    file.flush();
    file.close();
}

void PhoneBook::on_actionAbout_us_triggered()
{
    About a;
    a.exec();
}

void PhoneBook::on_btnDelAll_clicked()
{
    int rowAmount = ui->tablePhones->rowCount();
    for(int o = 0; o < rowAmount; o++)
        ui->tablePhones->removeRow(0);
    photoarray.clear();
    ui->btnDel->setEnabled(false);
}

void PhoneBook::on_pushButton_clicked()
{
    lbl = new PixmapLabel();
    lbl->setPixmap(pix);
    //lbl->setAlignment(Qt::AlignCenter);
    //lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lbl->setMinimumSize(320, 240);
    lbl->show();
}

void PhoneBook::on_tablePhones_clicked(const QModelIndex &index)
{
    int i = index.row();
    pix.load(photoarray[i]);
    ui->label_2->setPixmap(pix);
 //   ui->label_2->setBakkgroundRole(QPalette::Base); // Specialy made errors
    ui->label_2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->label_2->setScaledContents(true);
}
