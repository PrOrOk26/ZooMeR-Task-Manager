#include "db.h"
DB::DB(QString &database_name)
{
   *(this->database_name)=database_name;
   this->SetupModel();
   listOfQuerries.push_back("CREATE TABLE Group_info (Group_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL REFERENCES task_inf (Task_gr_id), Name VARCHAR(20) NOT NULL, Group_description VARCHAR(70))");
   listOfQuerries.push_back("CREATE TABLE IMP (Imp_Value INTEGER PRIMARY KEY NOT NULL REFERENCES task_inf (Importance))");
   listOfQuerries.push_back("CREATE TABLE task_data_time (Date DATE NOT NULL, Task_id TEXT NOT NULL REFERENCES task_inf (Task_id), Time TIME, PRIMARY KEY (Day,Task_id))");
   listOfQuerries.push_back("CREATE TABLE task_inf (Task_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, Task_name VARCHAR(50), Task_descr VARCHAR(70), Task_gr_id INTEGER, Importance INTEGER)");
   listOfQuerries.push_back("SELECT * FROM task_inf");
   listOfQuerries.push_back("INSERT INTO task_inf(Task_id,Task_name,Task_descr,Task_gr_id,Importance) VALUES(?,?,?,?,?)");
   listOfQuerries.push_back("INSERT INTO task_data_time(Date,Task_id,Time) VALUES(?,?,?)");
}
bool DB::InsertData(TypeOfInsert type,QList<QString>& listOfData)
{
    QSqlQuery query1,query2;
    switch(type)
    {
    case GROUP:
    {
        //
        //дописать все,что связяно с записью инфы о группе
    break;
    }
    case TASK:
    {
        query1.prepare(listOfQuerries[5]);
        int i;
        for(i=0;i<5;i++)
        query1.addBindValue(listOfData[i]);
        query1.exec();
        query2.prepare(listOfQuerries[6]);
        for(;i<listOfData.length()-1;i++)
        query2.addBindValue(listOfData[i]);
        query2.exec();
        break;
    }
    }
    return true;
}
void DB::SetupModel()
{
    QSqlDatabase databaseFTM=QSqlDatabase::addDatabase("QSQLITE");
    databaseFTM.setDatabaseName(":memory:");
    if(!databaseFTM.open())
    {
        QMessageBox::critical(0,("Cant open database"),("Unable to run..."),QMessageBox::Cancel);
        return;
    }
    else
    {
        QMessageBox::information(0,("database opened"),("running..."),QMessageBox::Apply);
    }
    QSqlQuery query;
    for(int i=0;i<4;i++)
    query.exec(listOfQuerries[i]);
}
QString DB::RetrieveData()
{
    QSqlQuery query;
    QString *resultOfQuerry=new QString();
    query.exec(listOfQuerries[4]);
    while(query.next())
    {
        *resultOfQuerry+=query.value(0).toString();
        *resultOfQuerry+=query.value(1).toString();
        *resultOfQuerry+=query.value(2).toString();
    }
    return *resultOfQuerry;
}
