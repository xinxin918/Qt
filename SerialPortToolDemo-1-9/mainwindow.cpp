#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //查找可用的串口
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortBox->addItem(serial.portName());
            serial.close();
        }
    }
    //设置波特率下拉菜单默认显示第0项
    ui->BaudBox->setCurrentIndex(0);
//    ui->PortBox->setEnabled(false);
//    ui->groupBox->setEnabled(false);
    ui->pushButton_set_hv_hight->setEnabled(false);
    ui->pushButton_set_hv->setEnabled(false);
    ui->pushButton_set_pinlv->setEnabled(false);
    ui->pushButton_open->setEnabled(false);
    ui->pushButton_close->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);
    ui->pushButton_up->setEnabled(false);
    ui->pushButton_down->setEnabled(false);
    ui->pushButton_ok_open->setEnabled(false);
    ui->pushButton_ok_close->setEnabled(false);
    ui->pushButton_ok_delay->setEnabled(false);
    ui->pushButton_hv_contr->setEnabled(false);
    ui->pushButton_hv_contr_2->setEnabled(false);
    ui->pushButton_set_yali_h->setEnabled(false);
    ui->pushButton_set_yali_low->setEnabled(false);
 //   ui->checkBox->setEnabled(false);


    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OpenSerialButton_clicked()
{
    if(ui->OpenSerialButton->text() == tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->PortBox->currentText());
        //打卡串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(ui->BaudBox->currentText().toInt());
        //设置数据位数
        switch (ui->BitBox->currentIndex())
        {
        case 8:
            serial->setDataBits(QSerialPort::Data8);
            break;
        default:
            break;
        }
        //设置校验位
        switch (ui->ParityBox->currentIndex())
        {
        case 0:
            serial->setParity(QSerialPort::NoParity);
            break;
        default:
            break;
        }
        //设置停止位
        switch (ui->BitBox->currentIndex())
        {
        case 1:
            serial->setStopBits(QSerialPort::OneStop);
            break;
        case 2:
            serial->setStopBits(QSerialPort::TwoStop);
        default:
            break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);

        //关闭设置菜单使能
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->BitBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->StopBox->setEnabled(false);
        ui->pushButton->setEnabled(false);

        ui->OpenSerialButton->setText(tr("关闭串口"));

        //连接信号槽
        QObject::connect(serial,&QSerialPort::readyRead,this,&MainWindow::ReadData);

        ui->pushButton_set_hv_hight->setEnabled(true);
        ui->pushButton_set_hv->setEnabled(true);
        ui->pushButton_set_pinlv->setEnabled(true);
        ui->pushButton_open->setEnabled(true);
        ui->pushButton_close->setEnabled(true);
        ui->pushButton_stop->setEnabled(true);
        ui->pushButton_up->setEnabled(true);
        ui->pushButton_down->setEnabled(true);
        ui->pushButton_ok_open->setEnabled(true);
        ui->pushButton_ok_close->setEnabled(true);
        ui->pushButton_ok_delay->setEnabled(true);
        ui->pushButton_hv_contr->setEnabled(true);
        ui->pushButton_hv_contr_2->setEnabled(true);
        ui->pushButton_set_yali_h->setEnabled(true);
        ui->pushButton_set_yali_low->setEnabled(true);
   //     ui->checkBox->setEnabled(true);
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        //恢复设置使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->OpenSerialButton->setText(tr("打开串口"));


        ui->pushButton_set_hv_hight->setEnabled(false);
        ui->pushButton_set_hv->setEnabled(false);
        ui->pushButton_set_pinlv->setEnabled(false);
        ui->pushButton_open->setEnabled(false);
        ui->pushButton_close->setEnabled(false);
        ui->pushButton_stop->setEnabled(false);
        ui->pushButton_up->setEnabled(false);
        ui->pushButton_down->setEnabled(false);
        ui->pushButton_ok_open->setEnabled(false);
        ui->pushButton_ok_close->setEnabled(false);
        ui->pushButton_ok_delay->setEnabled(false);
        ui->pushButton_hv_contr->setEnabled(false);
        ui->pushButton_hv_contr_2->setEnabled(false);
        ui->pushButton_set_yali_h->setEnabled(false);
        ui->pushButton_set_yali_low->setEnabled(false);
  //      ui->checkBox->setEnabled(false);

    }




}
//读取接收到的信息
void MainWindow::ReadData()
{
//    ui->textEdit->clear();
    QString buf;

    //读取串口缓冲区的所有数据给临时变量temp
    QByteArray temp = serial->readAll();

 //   ui->textEdit->setText(temp=temp.toHex());
//    qDebug()<<strlen(temp);
    if(!temp.isEmpty() && temp.count()==14 )
    {
            QString value,AD_h,AD_l,start_h,start_l,speed_h,speed_l,top1,top2;
            int top_flage=0;
            float dec ;
            bool ok;
            for(int i = 0; i < temp.count(); i++)
            {
                QString s;
                s.sprintf("%02x ", (unsigned char)temp.at(i));
                buf += s;
     //           qDebug()<<i<<s;
                switch(i){
                case 0:
                    top1=s[0];
                    top2=s[1];
                    break;
                case 1:
                    if(top1=="0" && top2=="6")
                    {
                        top1=s[0];
                        top2=s[1];
                        if(top1=="6" && top2=="b")
                        {
                            top_flage=1;
                        }
                    }
                    break;
                case 6:
                        AD_h=s[0];
                        AD_l=s[1];
                    break;
                case 7:
                    if(top_flage==1)
                    {
                        value=AD_l+s;
                        dec  = value.toInt(&ok, 16);    //16进制转换10 进制
                        if(AD_h=="0")
                        {
                            dec=dec*ui->lineEdit_k1->text().toFloat();
                            ui->lineEdit_AD_HV->setText( QString::number(dec));      //输出int类型
                        }
                        else if(AD_h=="1")
                        {
                            dec=dec*ui->lineEdit_k2->text().toFloat();
                            ui->lineEdit_AD_T->setText( QString::number(dec));
                        }
                        else if(AD_h=="2")
                        {
                            dec=dec*ui->lineEdit_k3->text().toFloat();
                            ui->lineEdit_AD_X->setText( QString::number(dec));
                        }
                        else if(AD_h=="3")
                        {
                            dec=dec*ui->lineEdit_k4->text().toFloat();
                            ui->lineEdit_AD_Y->setText( QString::number(dec));
                        }
                        else if(AD_h=="4")
                        {
                            dec=dec*ui->lineEdit_k5->text().toFloat();
                            ui->lineEdit_AD_Z->setText( QString::number(dec));
                        }

                    }

                    break;


                case 8:
                    start_h=s[0];
                    start_l=s[1];
                    break;
                case 9:
                    if(top_flage==1)
                    {
                        value=start_l+s;

                        dec  = value.toInt(&ok, 16);                         //16进制转换10 进制
                        dec=dec*ui->lineEdit_kyali->text().toFloat();
                        ui->lineEdit_yygdyl->setText( QString::number(dec));

                        if(start_h=="0")
                        {
                            ui->lineEdit_djzt->setText("静止");
                        }
                        else if(start_h=="1")
                        {
                            ui->lineEdit_djzt->setText("前进");
                        }
                        else if(start_h=="2")
                        {
                            ui->lineEdit_djzt->setText("后退");
                        }
                        else if(start_h=="8")
                        {
                            ui->lineEdit_open_close->setText("开腿");
                            ui->lineEdit_djzt->setText("停止");
                        }
                        else if(start_h=="9")
                        {
                            ui->lineEdit_open_close->setText("开腿");
                            ui->lineEdit_djzt->setText("前进");
                        }
                        else if(start_h=="a")
                        {
                            ui->lineEdit_open_close->setText("开腿");
                            ui->lineEdit_djzt->setText("后退");
                        }
                        else if(start_h=="c")
                        {
                            ui->lineEdit_open_close->setText("收腿");
                            ui->lineEdit_djzt->setText("停止");
                        }
                        else if(start_h=="d")
                        {
                            ui->lineEdit_open_close->setText("收腿");
                            ui->lineEdit_djzt->setText("前进");
                        }
                        else if(start_h=="e")
                        {
                            ui->lineEdit_open_close->setText("收腿");
                            ui->lineEdit_djzt->setText("后退");
                        }

                    }

                    break;

                case 10:
                    speed_h=s;
                    break;
                case 11:
                    if(top_flage==1)
                    {
                        speed_l=s;
                        value=speed_h+speed_l;
                        dec = value.toInt(&ok, 16);                         //16进制转换10 进制
                        ui->lineEdit_speed->setText( QString::number(dec));      //输出int类型

                    }
                    break;
                case 12:

                    break;
                case 13:

                    break;
                }

            }
            if(top_flage==1)
            {
                //将串口的数据追加在窗口的文本框中
                buf = buf.toUpper ();//转换为大写
        //        ui->textEdit->insertPlainText(buf);

                ui->textEdit->setText(buf);
            }
    //       qDebug()<<temp.count();
           top_flage=0;

    }




}




//发送按钮槽函数
void MainWindow::on_SendButton_clicked()
{
 //   serial->write(ui->textEdit_2->toPlainText().toLatin1());

    send_demo(ui->textEdit_2->toPlainText());

}


void MainWindow::on_pushButton_clicked()
{
    ui->PortBox->clear();
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortBox->addItem(serial.portName());
            serial.close();
        }
    }
    //设置波特率下拉菜单默认显示第0项
    ui->BaudBox->setCurrentIndex(0);
}



void MainWindow::on_pushButton_set_hv_clicked()
{
    send_demo_int("00 d6 02 00 30 90 ",ui->lineEdit_set_hv->text());
}

void MainWindow::on_pushButton_set_pinlv_clicked()
{
    send_demo_int_100("00 d6 02 00 40 90 ",ui->lineEdit_set_pinlv->text());
}

void MainWindow::on_pushButton_set_hv_hight_clicked()
{
    send_demo_int("00 d6 02 00 50 90 ",ui->lineEdit_set_hv_h->text());
}

void MainWindow::on_pushButton_open_clicked()   //开腿启动
{
    send_demo(ui->lineEdit_opentext->text());
 //   ui->pushButton_open->setEnabled(false);
    m_pTimer->start(1);
    time_set=1;
}

void MainWindow::on_pushButton_close_clicked()      //开腿停止
{
    send_demo(ui->lineEdit_close_text->text());
  //  ui->pushButton_open->setEnabled(true);
     m_pTimer->stop();
}

void MainWindow::on_pushButton_stop_clicked()       //停止
{
    send_demo(ui->lineEdit_stop_text->text());
}

void MainWindow::on_pushButton_up_clicked()         //前进
{
    send_demo(ui->lineEdit_up_text->text());
}

void MainWindow::on_pushButton_down_clicked()       //  后退
{
    send_demo(ui->lineEdit_down_text->text());
}

void MainWindow::on_pushButton_ok_open_clicked()    //电磁阀吸和
{
    send_demo(ui->lineEdit_ok_open->text());
}

void MainWindow::on_pushButton_ok_close_clicked()       //电磁阀打开
{
    send_demo(ui->lineEdit_ok_close->text());
}


void MainWindow::send_demo(QString sttr)                //发送串口函数
{
     ui->textEdit_step_show->setText(sttr);
 //    qDebug()<<sttr;
    QByteArray temp;
    if((serial!=NULL)&&(serial->isOpen()))
    {
            bool ok;
            char data;
            QString str;
            QStringList list;
            str = sttr;
            list = str.split(" ");
            for(int i = 0; i < list.count(); i++)
            {
                if(list.at(i) == " ")
                    continue;
                if(list.at(i).isEmpty())
                    continue;
                data = (char)list.at(i).toInt(&ok, 16);
                if(!ok){
                    QMessageBox::information(this, tr("提示消息"), tr("输入的数据格式有错误！"), QMessageBox::Ok);
                    return;
                }
                temp.append(data);
            }

        serial->write(temp);
    }
}

void MainWindow::send_demo_int(QString str1, QString str)      //串口发送数值函数
{

    QByteArray temp;
    QString ttext,temp16,temp_h,temp_l,temp_k;
    int x;
    if((serial!=NULL)&&(serial->isOpen()))
    {
        ttext=str;
   //     x=ttext.toInt();
        x=ttext.toFloat();
        temp16=QString("%1").arg(x,4,16,QLatin1Char('0')); //10进制转为16进制
        ui->textEdit_step_show->setText(temp16);
        temp16=ui->textEdit_step_show->toPlainText();

        if(temp16.length()==4)
        {
            temp_h=temp16.mid(0,2);
            temp_l=temp16.mid(2).append(" ");
            temp16=temp_l.append(temp_h);
        }


        temp16=str1+temp16;

    //    qDebug()<<temp16;

   //      ui->textEdit_step_show->setText(temp16);

         send_demo(temp16);

    }
}

void MainWindow::send_demo_int_100(QString str1, QString str)   //串口发送发送压力上下限函数
{

    QByteArray temp;
    QString ttext,temp16,temp_h,temp_l,temp_k;
    int x;
    if((serial!=NULL)&&(serial->isOpen()))
    {
        ttext=str;
   //     x=ttext.toInt();
        x=ttext.toFloat()*100;
        temp16=QString("%1").arg(x,4,16,QLatin1Char('0')); //10进制转为16进制
        ui->textEdit_step_show->setText(temp16);
        temp16=ui->textEdit_step_show->toPlainText();

        if(temp16.length()==4)
        {
            temp_h=temp16.mid(0,2);
            temp_l=temp16.mid(2).append(" ");
            temp16=temp_l.append(temp_h);
        }


        temp16=str1+temp16;

   //     qDebug()<<temp16;

  //       ui->textEdit_step_show->setText(temp16);

         send_demo(temp16);

    }
}

void MainWindow::send_demo_count(QString str1,float str)                //发送串口函数
{
    QByteArray temp;
    QString ttext,temp16,temp_h,temp_l,temp_k;
    int x;
    if((serial!=NULL)&&(serial->isOpen()))
    {
        ttext=str;
   //     x=ttext.toInt();
        x=ttext.toFloat();
        ttext.toFloat();
        temp16=QString("%1").arg(x,4,16,QLatin1Char('0')); //10进制转为16进制
        ui->textEdit_step_show->setText(temp16);
        temp16=ui->textEdit_step_show->toPlainText();

        if(temp16.length()==4)
        {
            temp_h=temp16.mid(0,2);
            temp_l=temp16.mid(2).append(" ");
            temp16=temp_l.append(temp_h);
        }


        temp16=str1+temp16;

         send_demo(temp16);

    }
}


void MainWindow::on_pushButton_ok_delay_clicked()
{
    if(time_flage==0)
    {
        time_temp=time_set;
         send_demo(ui->lineEdit_ok_close->text());
         time_flage=1;
  //      qDebug()<<1111<<ui->lineEdit_ok_delay->text().toInt();
    }


}
void MainWindow::on_pushButton_hv_contr_clicked()       //
{
//    if(ui->checkBox->isChecked()==true)
//    {
//        ui->pushButton_hv_contr->setText("停止自动控制");
//        send_demo("00 d6 02 00 52 90 01 00");
//         aotu_flage=1;
//         m_pTimer->start(1);
//    }
//    else if(ui->checkBox->isChecked()==false)
//    {
//        ui->pushButton_hv_contr->setText("开始使用");
//        send_demo("00 d6 02 00 52 90 02 00");
//         aotu_flage=0;
//         m_pTimer->stop();
//    }
//    if(ui->pushButton_hv_contr->text()=="开始自动控制")
//    {
//        ui->pushButton_hv_contr->setText("停止自动控制");
//        send_demo("00 d6 02 00 52 90 01 00");
//        aotu_flage=1;
//        m_pTimer->start(1);
//    }
//    else if (ui->pushButton_hv_contr->text()=="停止自动控制") {
//        ui->pushButton_hv_contr->setText("开始自动控制");
//        send_demo("00 d6 02 00 52 90 02 00");
//        aotu_flage=0;
//        m_pTimer->stop();
//    }
        send_demo("00 d6 02 00 52 90 01 00");
        aotu_flage=1;
        m_pTimer->start(1);
}
void MainWindow::on_pushButton_hv_contr_2_clicked()
{
  //  send_demo_count("00 6d",15);
        send_demo("00 d6 02 00 52 90 02 00");
        aotu_flage=0;
        m_pTimer->stop();
}


void MainWindow::handleTimeout()
{

    ui->lineEdit_time->setText(  QString::number(time_set/1000));
    time_set++;

    if(time_flage==1  &&  time_set - time_temp - ui->lineEdit_ok_delay->text().toInt() >10 ){

        send_demo(ui->lineEdit_ok_open->text());
        time_flage=0;
    }
//    if(sleep_flage==1  &&  time_set - sleep_temp - 2000 >10 ){

//      //  send_demo(ui->lineEdit_ok_open->text());
//        sleep_flage=0;
//        if (ui->checkBox->isChecked()==true && aotu_flage==1) {

//            if( (ui->lineEdit_yygdyl->text().toFloat()) > (ui->lineEdit_set_hv_hight->text().toFloat()) )
//            {
//                qDebug()<<1111;

//                send_demo(ui->lineEdit_close_text->text());
//            }
//            else if(  (ui->lineEdit_yygdyl->text().toFloat()) < (ui->lineEdit_set_hv_low->text().toFloat()) )
//            {
//                 qDebug()<<2222;
//                 send_demo(ui->lineEdit_opentext->text());

//            }

//         }
//    }

}

void MainWindow::sleep(int sec)
{
  //  sec=time_set;

    if(sleep_flage==0)
    {
        sleep_temp=time_set;
        sleep_temp2=sec;
        sleep_flage=1;
    }
}

void MainWindow::on_pushButton_set_yali_h_clicked()
{
  send_demo_int_100("00 d6 02 00 50 90 ",ui->lineEdit_set_hv_hight->text());
}


void MainWindow::on_pushButton_set_yali_low_clicked()
{
   send_demo_int_100("00 d6 02 00 51 90 ",ui->lineEdit_set_hv_low->text());
}

