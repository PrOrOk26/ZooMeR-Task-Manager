#ifndef ADDGROUPDIALOG_H
#define ADDGROUPDIALOG_H

#include <QWidget>
#include <QtWidgets>
#include <QSettings>

class AddGroupDialog: public QDialog {
    Q_OBJECT
private:
    QLabel* lGroupName;
    QLineEdit* leGroupName;
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
    AddGroupDialog(QWidget *parent = 0);
    void ShowWarning();
public slots:
    void isFilled();
};

#endif // ADDGROUPDIALOG_H
