#ifndef WIDGET_H
#define WIDGET_H

#include "addobjdialog.h"
#include "addgroupdialog.h"
#include "db.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QTreeView* treeTask;
    QTreeView* treeDoneTask;

    QStandardItemModel* actmodel;
    QStandardItemModel* donemodel;
private:
    Ui::Widget *ui;
    //myFunc
    void InitializeComponent();
    void SetupLayouts();
    void ConnectSignals();
    void CreateSystemTray();
    void EmitCheckSignal();
    void CheckRemember();

    void WriteSettings();
    void ReadSettings();

    void Repeat(QString id);
    void ShowRemember(QString remember, qint64 n);
    void BuildModel(QStandardItemModel* model, int donenumb);
    void SetupTree();
    void UpdateTree();
    //myVar
    QPushButton* buttonAddObj;
    QPushButton* buttonAddGroup;
    QRadioButton* radioAll;
    QRadioButton* radioDay;
    QRadioButton* radioWeek;
    QRadioButton* radioMonth;

    QGroupBox* groupOfRadio;
    QTabWidget* tabs;

    QVBoxLayout* layoutRightSide;
    QVBoxLayout* layoutLeftSide;
    QVBoxLayout* layoutForRadio;
    QHBoxLayout* layForButton;
    QBoxLayout* mainLayout;

    QSettings* settings;

    DB* database;

    QSystemTrayIcon* stTrayIcon;
    QMenu* mContMenu;
protected:
    virtual void closeEvent(QCloseEvent* pe);
public slots:
    void slotOpenDialogAddGroup();
    void slotOpenDialogAddObj();
    void slotRadioButtonClicked();
    void slotItemChanged(QStandardItem* item);
    void slotStartInsert(QString group_name);
    void slotShowHide();
    void slotTrayActivated(QSystemTrayIcon::ActivationReason reason);
signals:
    void ShowWarning(TypeOfWarning type);
    void AcceptGroup();
};

#endif // WIDGET_H
