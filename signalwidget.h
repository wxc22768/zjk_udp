#ifndef SIGNALWIDGET_H
#define SIGNALWIDGET_H

#include <QWidget>

namespace Ui {
class SignalWidget;
}

class SignalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SignalWidget(QWidget *parent = nullptr);

    void Set_signalstrength(int newsignalstrenght);
    void paintEvent(QPaintEvent *event);

    ~SignalWidget();

private:
    Ui::SignalWidget *ui;
    int m_Signalstrength;
};

#endif // SIGNALWIDGET_H
