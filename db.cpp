#include "db.h"

DB::DB(QString db_name) : database_name(db_name)
{
    //0
    createQueries.push_back("CREATE TABLE Group_info ("
                             "Group_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                             "Group_name VARCHAR(20) NOT NULL UNIQUE)"
                             );
    //1
    createQueries.push_back("CREATE TABLE Task_info ("
                             "Task_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                             "Task_name VARCHAR(50),"
                             "Task_descr VARCHAR(70),"
                             "Task_gr_id INTEGER,"
                             "Importance INTEGER,"
                             "Remember INTEGER,"
                             "FOREIGN KEY(Task_gr_id) REFERENCES Group_info(Group_id))"
                             );
    //2
    createQueries.push_back("CREATE TABLE Task_date_time ("
                             "Task_id INTEGER,"
                             "Date DATE,"
                             "Time TIME,"
                             "PRIMARY KEY (Task_id, Date, Time),"
                             "FOREIGN KEY(Task_id) REFERENCES Task_info(Task_id))"
                             );
    //3
    createQueries.push_back("CREATE TABLE Task_done_rep ("
                             "Task_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                             "Done INTEGER NOT NULL,"
                             "Repeat INTEGER NOT NULL)");

    //0
    insertQueries.push_back("INSERT INTO Group_info(Group_name) VALUES (?)");
    //1
    insertQueries.push_back("INSERT INTO Task_done_rep(Done, Repeat) VALUES (?, ?)");
    //2
    insertQueries.push_back("INSERT INTO Task_info(Task_name, Task_descr, Task_gr_id, Importance, Remember) VALUES (?,?,?,?,?)");
    //3
    insertQueries.push_back("INSERT INTO Task_date_time(Task_id, Date, Time) VALUES (?, ?, ?)");
   SetupModel();
}

void DB::SetupModel()
{
    QSqlDatabase databaseFTM = QSqlDatabase::addDatabase("QSQLITE");
    databaseFTM.setDatabaseName(database_name);
    if(!databaseFTM.open()) {
        QMessageBox::critical(0,("Database"),
                    ("Can't open"),QMessageBox::Cancel);
    }
    QSqlQuery query;
    for(int i = 0; i < createQueries.size(); i++) {
       query.exec(createQueries[i]);
    }
    query.exec("INSERT INTO Group_info(Group_id, Group_name) VALUES (0, 'Разное')");
}

bool DB::InsertData(TypeOfInsert type, QList<QVariant> &listOfData)
{
    QSqlQuery query;
    switch(type)
    {
        case GROUP:
        {
            query.prepare(insertQueries[0]);
            query.addBindValue(listOfData[0].toString());
            if(!query.exec()) {
                 return false;
            }
        break;
        }
        case TASK:
        {
            bool success = true;
            query.prepare(insertQueries[1]);
            query.addBindValue(0); // Done; задача еще не выполнена
            query.addBindValue(listOfData[0].toInt()); //Repeat; повторять каждые...дней
            query.exec();
            query.prepare(insertQueries[2]);
            query.addBindValue(listOfData[1].toString()); //Task_name
            query.addBindValue(listOfData[2].toString()); //Task_descr
            query.addBindValue(listOfData[3].toInt());    //Task_gr_id
            query.addBindValue(listOfData[4].toInt());    //Importance
            query.addBindValue(listOfData[5].toInt()); //Remember; за сколько дней напомнить
            query.exec();
            int n = query.lastInsertId().toInt();
            query.prepare(insertQueries[3]);
            query.addBindValue(n); //Task_id
            query.addBindValue(listOfData[6].toDate()); //Date
            query.addBindValue(listOfData[7].toTime()); //Time
            query.exec();
        }
    }
    return true;
}
