#ifndef BD_H
#define BD_H

#include <QWidget>
#include <QUdpSocket>
#include <QMap>

#include "YIMAENCHEAD.H"
#include "barvolume.h"

QT_BEGIN_NAMESPACE
namespace Ui { class bd; }
QT_END_NAMESPACE

class bd : public QWidget
{
    Q_OBJECT

public:
    bd(QWidget *parent = nullptr);
    ~bd();

    void dealMsg();
    void Show_data();
    QString Send_bw(QString str);
    void Show_signal();
    QByteArray Xor_exam(QString whole);
    void Set_all(QString buf);
    void Set_signal(QString str);

    M_POINT Get_loca();
    QString Get_time();
    QString Get_info(int index);
    int Get_signal(int index);

private slots:
    void on_btn_send_clicked();

    void on_btn_info_get_clicked();

    void on_btn_signal_get_clicked();

    void on_btn_getic_clicked();

    void on_btn_get_signal_clicked();

private:

    Ui::bd *ui;
    QMap<int,QString> m_Info_map;
    QMap<int,int> m_Signal_map;

    QUdpSocket *udpsocket;
};
#endif // WIDGET_H
