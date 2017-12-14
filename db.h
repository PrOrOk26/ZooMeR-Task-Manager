#ifndef DB_H
#define DB_H
#include "QString"
#include "QtSQL"
#include <QMessageBox>
#include <QList>

enum TypeOfInsert{TASK,GROUP};

class DB
{
public:
    DB (QString db_name);
    ~DB();

    void SetupModel();  //создается БД
    QStringList GetGroupList();
    bool InsertData(TypeOfInsert type, QList<QVariant> &listOfData);
    //в БД добавляются записи с инфой о задачах
private:
   QString database_name;
   QList<QString> createQueries;
   QList<QString> insertQueries;
};

#endif // DB_H
