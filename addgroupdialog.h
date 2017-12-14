#ifndef ADDGROUPDIALOG_H
#define ADDGROUPDIALOG_H

#include <QWidget>
#include <QtWidgets>
#include <QSettings>

enum TypeOfWarning{EMPTY, NOT_UN};

class AddGroupDialog: public QDialog {
    Q_OBJECT
private:
    QLabel* lGroupName;

    QPushButton* pAdd;
    QPushButton* pCancel;

    QLabel* lWarning;

    //Layouts
    QVBoxLayout* mlay;
    QHBoxLayout* layForButton;

    //Functions
    void InitializeComponent();
    void SetupLayouts();
    void ConnectSignals();
public:
    QLineEdit* leGroupName;

    AddGroupDialog(QWidget *parent = 0);

public slots:
    void isFilled();
    void slotShowWarning(TypeOfWarning type);
    void slotHideWarning();
signals:
    void StartInsert(QString group_name);
};

#endif // ADDGROUPDIALOG_H
