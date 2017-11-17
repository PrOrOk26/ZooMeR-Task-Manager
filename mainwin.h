#ifndef WIDGET_H
#define WIDGET_H

#include "addobjdialog.h"
#include "addgroupdialog.h"

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
    void CreateSystemTray();

    void ShowAllObj();
    void ShowDayObj();
    void ShowWeekObj();
    void ShowMonthObj();

    void WriteSettings();
    void ReadSettings();

    void ShowRemember();
    //myVar
    QPushButton* buttonAddObj;
    QPushButton* buttonAddGroup;
    QRadioButton* radioAll;
    QRadioButton* radioDay;
    QRadioButton* radioWeek;
    QRadioButton* radioMonth;

    QGroupBox* groupOfRadio;

    QVBoxLayout* layoutRightSide;
    QVBoxLayout* layoutLeftSide;
    QVBoxLayout* layoutForRadio;
    QHBoxLayout* layForButton;
    QBoxLayout* mainLayout;

    QSettings* settings;

    QSystemTrayIcon* stTrayIcon;
    QMenu* mContMenu;
protected:
    virtual void closeEvent(QCloseEvent* pe);
public slots:
    void slotOpenDialogAddGroup();
    void slotOpenDialogAddObj();
    void slotRadioButtonClicked();

    void slotShowHide();
};

#endif // WIDGET_H
