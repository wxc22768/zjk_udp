#include "signalwidget.h"
#include "ui_signalwidget.h"
#include <QPainter>

SignalWidget::SignalWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignalWidget)
{
    ui->setupUi(this);
}

void SignalWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    float hUint = this->height()/19.0;
    int h = hUint * 4;

    float wUint = hUint * 4;
    float x = (this->width()-hUint)/2.0;

    QColor haveRed = QColor(237,28,36);
    QColor haveOrange = QColor(255,127,39);
    QColor haveGreen = QColor(44,230,99);
    QColor noSignal = Qt::gray;

    QColor colorOne, colorTwo, colorThree, colorFour;
    colorOne = noSignal;
    colorTwo = noSignal;
    colorThree = noSignal;
    colorFour = noSignal;

    switch(m_Signalstrength){
    case 4:
        colorFour = haveGreen;
        colorThree = haveGreen;
        colorTwo = haveGreen;
        colorOne = haveGreen;
        break;
    case 3:
        colorThree = haveGreen;
        colorTwo = haveGreen;
        colorOne = haveGreen;
        break;
    case 2:
        colorTwo = haveOrange;
        colorOne = haveOrange;
        break;
    case 1:
        colorOne = haveRed;
        break;
    default:break;
    }

    p.setPen(colorOne);
    p.setBrush(colorOne);
    p.drawRoundRect(x, this->height()-hUint*4,wUint,h);

    p.setPen(colorTwo);
    p.setBrush(colorTwo);
    p.drawRoundRect(x, this->height()-hUint*9,wUint,h);

    p.setPen(colorThree);
    p.setBrush(colorThree);
    p.drawRoundRect(x, this->height()-hUint*14,wUint,h);

    p.setPen(colorFour);
    p.setBrush(colorFour);
    p.drawRoundRect(x, this->height()-hUint*19,wUint,h);

}

void SignalWidget::Set_signalstrength(int newsignalstrenght){
    m_Signalstrength = newsignalstrenght;
    this->update();
}

SignalWidget::~SignalWidget()
{
    delete ui;
}
