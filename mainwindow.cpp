#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDirIterator>
#include <QMessageBox>
#include <QDebug>
#include <cmath>
#include "funcs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(on_pushButton_2_clicked()));
}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}


void MainWindow::on_toolButton_clicked()
{
    QString str = QFileDialog::getExistingDirectory(0, "Выбор папки", "");
    ui->label_8->setText(str);
}



void MainWindow::on_pushButton_2_clicked()
{
    QString value = ui->lineEdit_Value->text();
    QDirIterator it(ui->label_12->text(), QStringList() << ui->lineEdit_Mask->text(), QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        short d = 0;
        QFile file(it.next());
        QFileInfo fileInfo(file.fileName());
        QString path = ui->label_8->text() + "/" + fileInfo.baseName() + '.' + fileInfo.completeSuffix();
        while (QFile::exists(path) and ui->radioButton_AddN->isChecked())
        {
            d++;
            path = ui->label_8->text() + "/" + fileInfo.baseName() + char(d + '0') + '.' + fileInfo.completeSuffix();
        }
        QFile file_out(path);
        file.open(QIODevice::ReadOnly);
        qDebug() << file.fileName() << "is opened";
        file_out.open(QIODevice::WriteOnly);
        QDataStream out(&file_out);
        qDebug() << file_out.fileName() << "is opened";

        QByteArray data = file.read(8);

        QBitArray bit_data = bytesToBits(data);
        QBitArray bit_val(64);
        QString valuee = QString("%1").arg(value.toULongLong(nullptr, 16), 5, 2, QChar('0'));
        for (qsizetype i = 0; i < valuee.length(); i++)
        {
            if (valuee[i] == '1')
            {
                bit_val.setBit(i, true);
            }
        }
        qDebug() << bit_data;
        qDebug() << bit_val;
        QBitArray bit_res(64);
        if (ui->radioButton_AND->isChecked())
        {
            bit_res = bit_data & bit_val;
        }
        else if (ui->radioButton_OR->isChecked())
        {
            bit_res = bit_data | bit_val;
        }
        else if (ui->radioButton_XOR->isChecked())
        {
            bit_res = bit_data ^ bit_val;
        }
        else
        {
            qDebug() << "Пользователь не выбрал операцию";
        }

        QByteArray bytes;
        bytes.resize(bit_res.size()/8);

        char temp = bit_res[0] * 128;
        for(int b = 1; b < bit_res.count(); ++b)
        {
            temp += bit_res[b] * std::pow(2, 7 - (b % 8));

            if (b % 8 == 7)
            {
                bytes[b/8] = temp;
                temp = 0;
            }
        }
        qDebug() << bit_res;
        file_out.write(bytes);
        file_out.write(file.readAll());

        file.close();
        file_out.close();
        if (ui->radioButton_Del->isChecked())
        {
            file.remove();
        }
    }
    if (ui->radioButton_Per->isChecked())
    {
        timer->start(60000 * ui->lineEdit_Period->text().toInt());
    }
    QMessageBox::information(this, "Успешно","Все файлы модифицированы");
}


void MainWindow::on_toolButton_2_clicked()
{
    QString str = QFileDialog::getExistingDirectory(0, "Выбор папки", "");
    ui->label_12->setText(str);
}


void MainWindow::on_radioButton_AND_clicked()
{
    ui->radioButton_XOR->setChecked(false);
    ui->radioButton_OR->setChecked(false);
}


void MainWindow::on_radioButton_OR_clicked()
{
    ui->radioButton_AND->setChecked(false);
    ui->radioButton_XOR->setChecked(false);
}


void MainWindow::on_radioButton_XOR_clicked()
{
    ui->radioButton_AND->setChecked(false);
    ui->radioButton_OR->setChecked(false);
}


void MainWindow::on_radioButton_Del_clicked()
{
    ui->radioButton_NoDel->setChecked(false);
}


void MainWindow::on_radioButton_NoDel_clicked()
{
    ui->radioButton_Del->setChecked(false);
}

