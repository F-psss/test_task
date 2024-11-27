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
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}

void MainWindow::slotTimerAlarm()
{
    /* Слот таймера, чтобы после его запуска при иземенинии
     * "Переодичность" на "Нет (остановка)" таймер действительно
     * ни разу не работал
     */
    if (ui->radioButton_NoPer->isChecked())
    {
        timer->stop();
    }
    else
    {
        MainWindow::on_pushButton_2_clicked();
    }
}

void MainWindow::on_toolButton_In_clicked()
{
    QString str = QFileDialog::getExistingDirectory(0, "Выбор папки", "");
    ui->label_12->setText(str);
}

void MainWindow::on_toolButton_Out_clicked()
{
    QString input_directory = QFileDialog::getExistingDirectory(0, "Выбор папки", "");
    ui->label_8->setText(input_directory);
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

void MainWindow::on_pushButton_2_clicked()
{
    /*
     * Функция вызывается при нажатии кнопки Выполнить,
     * включает в себя всю логику изменения файлов
     */

    int flag = 0;
    if (ui->radioButton_AND->isChecked())
    {
        flag = 1;
    }
    else if (ui->radioButton_OR->isChecked())
    {
        flag = 2;
    }
    else if (ui->radioButton_XOR->isChecked())
    {
        flag = 3;
    }
    else
    {
        QMessageBox::information(this, "Внимание","Укажите операцию");
        qDebug() << "Пользователь не выбрал операцию";
        return;
    }

    //Считывание и перевод в QBitArray 8-битной переменной
    QString value = ui->lineEdit_Value->text();
    QBitArray var_bit(64);
    QString value_str = QString("%1").arg(value.toULongLong(nullptr, 16), 5, 2, QChar('0'));
    for (qsizetype i = 0; i < value_str.length(); i++)
    {
        if (value_str[i] == '1')
        {
            var_bit.setBit(i, true);
        }
    }

    //Создаю итератор с файлами подходящими под маску
    QDirIterator it(ui->label_12->text(), QStringList() << ui->lineEdit_Mask->text(), QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {

        short postfile = 0;
        QFile file(it.next());
        QFileInfo fileInfo(file.fileName());
        QString path = ui->label_8->text() + "/" + fileInfo.baseName() + '.' + fileInfo.completeSuffix();
        //Если выходные файлы сушествуют и не заменяются, то дописываю уникальный символ
        if (ui->radioButton_AddN->isChecked())
        {
            while (QFile::exists(path))
            {
                postfile++;
                path = ui->label_8->text() + "/" + fileInfo.baseName() + char(postfile + '0') + '.' + fileInfo.completeSuffix();
            }
        }
        QFile file_out(path);

        file.open(QIODevice::ReadOnly);
        qDebug() << file.fileName() << "is opened";
        file_out.open(QIODevice::WriteOnly);
        QDataStream out(&file_out);
        qDebug() << file_out.fileName() << "is opened";

        QByteArray input_data_byte = file.readAll();
        QBitArray input_data_bit = bytesToBits(input_data_byte);
        qDebug() << input_data_bit;
        qDebug() << var_bit;

        QBitArray bit_res(input_data_bit.size());
        for (int i = 0; i < input_data_bit.size(); ++i)
        {
            switch (flag) {
                case 1:
                    bit_res.setBit(i, input_data_bit[i] & var_bit[i%64]);
                    break;
                case 2:
                    bit_res.setBit(i, input_data_bit[i] | var_bit[i%64]);
                    break;
                case 3:
                    bit_res.setBit(i, input_data_bit[i] ^ var_bit[i%64]);
                    break;
            }
        }

        //Перевожу QbitArray в выходной QByteArray
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
        //Цикл for в качестве небольшого костыля, все встроенные и найденные функции меняют порядок бит

        qDebug() << bit_res;
        file_out.write(bytes);

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
