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
    DB(QString &database_name);
    ~DB();
    bool InsertData(TypeOfInsert type,QList<QString>& listOfData);  //в БД добавляются записи с инфой о задачах
    void SetupModel();  //создается БД
    QString RetrieveData(); //тест для проверки работоспособности БД
private:
   QString* database_name;
   QList<QString>listOfQuerries;
};

#endif // DB_H
