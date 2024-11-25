#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_toolButton_clicked();

    void on_pushButton_2_clicked();

    void on_toolButton_2_clicked();

    void on_radioButton_AND_clicked();

    void on_radioButton_OR_clicked();

    void on_radioButton_XOR_clicked();

    void on_radioButton_Del_clicked();

    void on_radioButton_NoDel_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
};
#endif // MAINWINDOW_H
