#include "bd.h"
#include "ui_bd.h"
#include "YIMAENCHEAD.H"
#include <QHostAddress>
#include <QDebug>
#include <string.h>
#include <QBitArray>

bd::bd(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::bd)
{
    ui->setupUi(this);
    udpsocket = new QUdpSocket(this);

    udpsocket->bind(28460);
    setWindowTitle("服务器端口为28460");
    m_Signal_map[1] = 1;
    m_Signal_map[2] = 4;
    m_Signal_map[3] = 3;
    Show_signal();
    connect(udpsocket, &QUdpSocket::readyRead,this, &bd::dealMsg);
}

bd::~bd()
{
    delete ui;
}


void bd::Set_all(QString buf){
//    int len = buf.count("$");
//    for(int i=0;i<len+1;i++)
    {
        //QString qst_buf_one = buf.split("$")[i];
        QString qst_buf_one = buf;

        if(qst_buf_one.contains("GNGGA") && qst_buf_one.contains("*")){//显示时间地点
            //0:时分秒;1:度（纬度）;2:分（纬度）;3:"E/W";4:度（经度）;5:分（经度）;6:”N/S;
            m_Info_map[0] = qst_buf_one.split(',')[1].mid(0, 6);//时
            m_Info_map[2] = qst_buf_one.split(',')[2].right(8);//分（纬度）eg:12.12345
            m_Info_map[1] = qst_buf_one.split(',')[2].replace(m_Info_map[2],"");//度（纬度）
            m_Info_map[3] = qst_buf_one.split(',')[3];//“N/S”纬度方向
            m_Info_map[5] = qst_buf_one.split(',')[4].right(8);//分（经度）
            m_Info_map[4] = qst_buf_one.split(',')[4].replace(m_Info_map[5],"");//度（纬度）
            m_Info_map[6] = qst_buf_one.split(',')[5];//“E/W”经度方向
            //continue;
        }
        if(qst_buf_one.contains("GNVTG") && qst_buf_one.contains("*")){//无用信息
            //continue;
        }
        if(qst_buf_one.contains("BDICI") && qst_buf_one.contains("*")){//卡号信息
            ui->textelse->setText("卡号为：" + qst_buf_one.split(",")[1]);
            //continue;
        }
        if(qst_buf_one.contains("BDBSI") && qst_buf_one.contains("*")){//信号强度序列：eg：$BDBSI,02,08,0,3,0,0,0,0,0,3,0,0*54
            Set_signal(qst_buf_one);
            //continue;
        }
        if(qst_buf_one.contains("TXA") && qst_buf_one.contains("*")){//其他信息
            qDebug()<<qst_buf_one;
            QString baowen = qst_buf_one.split(',')[4];
            //ui->textelse->setText(qst_buf_one);
            //m_Info_map[7] = qst_buf_one;//报文信息
            ui->textelse->setText("接收到报文：" + baowen);
            m_Info_map[7] = baowen;//报文信息
            qDebug() << "fffff";
        }
    }
}

void bd::dealMsg(){
    char buf[1024] = {0};
    QHostAddress address;
    quint16 port;
    qint64 len = udpsocket->readDatagram(buf, sizeof(buf), &address, &port);

    quint32 temp = address.toIPv4Address();
    address = QHostAddress(temp);
    if(len >0){
        QString str = QString("[%1:%2] %3")
                .arg(address.toString())
                .arg(port)
                .arg(buf);

        QString qst_buf = QString::fromLocal8Bit(buf);
        Set_all(qst_buf);
        Show_data();
        Show_signal();
    }
    M_POINT my_point = Get_loca();

    qDebug()<<my_point.x << my_point.y << Get_time();
}

void bd::on_btn_send_clicked()
{
    QString ip = ui->ip->text();
    quint16 port = ui->port->text().toInt();

    QString str = ui->textsend->toPlainText();

    QString bw = Send_bw(str);
    udpsocket->writeDatagram(bw.toUtf8(), QHostAddress(ip), port);
    qDebug()<<"发送："<<bw.toUtf8();
}

void bd::Show_data(){
    QString loc_j;
    QString loc_w;
    QString time;
    loc_w +="纬度：" + m_Info_map[1] + "°" + m_Info_map[2] + "′" + m_Info_map[3];
    loc_j +="经度：" + m_Info_map[4] + "°" + m_Info_map[5] + "′" + m_Info_map[6];
    ui->textedit_2->setText(loc_j);
    ui->textEdit_3->setText(loc_w);
    auto mhour = m_Info_map[0].mid(0,2).toUInt();
    auto mmin = m_Info_map[0].mid(2,2).toUInt();
    auto msec = m_Info_map[0].mid(4,2).toUInt();
    time += QString::number(mhour+8,10) + "时 "+QString::number(mmin,10) + "分 "+QString::number(msec,10) + "秒 ";
    ui->textedit->setText(time);
}


QString bd::Send_bw(QString str){
    QString whole_text = "$CCTXA,0229886,1,2," + str;
    QByteArray result = Xor_exam(whole_text);
    QByteArray end = QByteArray("*") + result;
    whole_text = whole_text + QString(end);
    qDebug()<<whole_text;
    return whole_text;
}

void bd::Set_signal(QString str)
{
    ui->bv_1->setValue(str.split(",")[3].toInt());

    m_Signal_map[1] = str.split(",")[3].toInt();
    m_Signal_map[2] = str.split(",")[4].toInt();
    m_Signal_map[3] = str.split(",")[5].toInt();
    m_Signal_map[4] = str.split(",")[6].toInt();
    m_Signal_map[5] = str.split(",")[7].toInt();
    m_Signal_map[6] = str.split(",")[8].toInt();
    m_Signal_map[7] = str.split(",")[9].toInt();
    m_Signal_map[8] = str.split(",")[10].toInt();
    m_Signal_map[9] = str.split(",")[11].toInt();
    m_Signal_map[10] = str.split(",")[12].toInt();
}

void bd::Show_signal(){//"BDBSI,02,08,0,3,0,0,0,0,0,3,0,0*54"
    ui->bv_1->setValue(m_Signal_map[1]);
    ui->bv_2->setValue(m_Signal_map[2]);
    ui->bv_3->setValue(m_Signal_map[3]);
    ui->bv_4->setValue(m_Signal_map[4]);
    ui->bv_5->setValue(m_Signal_map[5]);
    ui->bv_6->setValue(m_Signal_map[6]);
    ui->bv_7->setValue(m_Signal_map[7]);
    ui->bv_8->setValue(m_Signal_map[8]);
    ui->bv_9->setValue(m_Signal_map[9]);
    ui->bv_10->setValue(m_Signal_map[10]);
}

QByteArray bd::Xor_exam(QString whole){
    QByteArray result;
    for(QString::iterator it=whole.begin();it!=whole.end();it++){
        if(*it == "$"){
            continue;
        }
        if(*it == "*"){
            break;
        }
        QString str_it = (*it);
        QByteArray qba_it = str_it.toLatin1();
        result[0] = result[0]^qba_it[0];
    }
    return result.toHex().toUpper();
}


//返回经纬度结构体的函数接口
M_POINT bd::Get_loca(){
    int x = m_Info_map[4].toInt()*10000000;
    double xpoint = m_Info_map[5].toDouble()/60;
    x += int(xpoint*10000000);
    int y = m_Info_map[1].toInt()*10000000;
    double ypoint = m_Info_map[2].toDouble()/60;
    y += int(ypoint*10000000);
    M_POINT beidou_point(x,y);
    return beidou_point;
}

//返回时间的函数接口
QString bd::Get_time(){
    qDebug() << m_Info_map[0];
    return m_Info_map[0];
}

int bd::Get_signal(int index){
    return m_Signal_map[index];
}

QString bd::Get_info(int index){
    return m_Info_map[index];
}

void bd::on_btn_info_get_clicked()
{
    int index = ui->info_index->text().toInt();
    ui->info_show->setText(Get_info(index));
}

void bd::on_btn_signal_get_clicked()
{
    int index = ui->signal_index->text().toInt();
    ui->signal_show->setText(QString::number(Get_signal(index),10));
}

void bd::on_btn_getic_clicked()
{
    QString ip = ui->ip->text();
    quint16 port = ui->port->text().toInt();

    QString str = QString("$CCICA,0,00*7B");
    udpsocket->writeDatagram(str.toUtf8(), QHostAddress(ip), port);
}

void bd::on_btn_get_signal_clicked()
{
    QString ip = ui->ip->text();
    quint16 port = ui->port->text().toInt();

    QString str = QString("$CCRMO,BSI,2,0*26");
    udpsocket->writeDatagram(str.toUtf8(), QHostAddress(ip), port);
    qDebug()<<"信号获取请求已发送";
}
