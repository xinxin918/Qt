#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QTextCodec>
#include <QRegExp>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_OpenSerialButton_clicked();

    void ReadData();

//    char extractLow(QString str);
 //   char extractHigh(QString str);
 //   void dec2hex(int x,char s[]);

    void on_SendButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_set_hv_clicked();

    void on_pushButton_open_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_down_clicked();

    void send_demo(QString sttr);
    void send_demo_int(QString str1, QString str);
    void send_demo_int_100(QString str1, QString str);
    void send_demo_count(QString str1,float str);
    void on_pushButton_set_pinlv_clicked();

    void on_pushButton_set_hv_hight_clicked();

    void on_pushButton_ok_open_clicked();

    void on_pushButton_ok_close_clicked();

    void on_pushButton_ok_delay_clicked();

//    void sleep(unsigned int msec);

    void on_pushButton_hv_contr_clicked();

    void on_pushButton_set_yali_h_clicked();

    void on_pushButton_set_yali_low_clicked();

    void handleTimeout();  //超时处理函数
    void sleep(int sec);
    void on_pushButton_hv_contr_2_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTimer  *m_pTimer;
 float time_temp=0;
    int time_flage=0;
    int time_set=0;
    int aotu_flage=0;
   int sleep_flage=0;
   int sleep_temp=0;
   int sleep_temp2=0;
};

#endif // MAINWINDOW_H
