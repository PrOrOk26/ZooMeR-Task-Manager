#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets>
#include "addobjdialog.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    //myFunc
    void InitializeComponent();
    void SetupLayouts();
    void ConnectSignals();

    void ShowAllObj();
    void ShowDayObj();
    void ShowWeekObj();
    void ShowMonthObj();

    //myVar
    QPushButton* buttonAddObj;
    QRadioButton* radioAll;
    QRadioButton* radioDay;
    QRadioButton* radioWeek;
    QRadioButton* radioMonth;

    QGroupBox* groupOfRadio;

    QVBoxLayout* layoutRightSide;
    QVBoxLayout* layoutLeftSide;
    QVBoxLayout* layoutForRadio;

    QBoxLayout* mainLayout;

public slots:
    void slotOpenDialogAddObj();
    void slotRadioButtonClicked();
};

#endif // WIDGET_H
