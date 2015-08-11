#ifndef DBMANAGER_H
#define DBMANAGER_H

#include<QtSql/QSqlDatabase>
#include<QDebug>
#include<QSqlTableModel>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QSqlError.h>
#include<QDir.h>
#include<QStandardItemModel>

class DBManager
{
private:
	static DBManager *manager;
	//DBManager(){}
	QSqlDatabase db;
public:
	static DBManager* Instance()
	{
		if (manager == NULL)
		{
			manager = new DBManager();
			manager->ConnectToDB();
		}
		return manager;
	}
	static void SetManagerToNull()
	{
		manager = NULL;
	}
	bool ConnectToDB()
	{
		db = QSqlDatabase::database("test", false);
		//if (!db.isValid())
		//{
			db = QSqlDatabase::addDatabase("QODBC", "test");
			QString path = QDir().currentPath();
			db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=" + path + "\\TreesModel.mdb");
			if (db.open())
				return true;
			else
			{
				qDebug() << db.lastError().text() << endl;
				return false;
			}
		//}
		//else return true;
	}
    //modelname--->modelpath
	QString FindPathByTreeName(QString name)
	{
		db = QSqlDatabase::database("test", false);
        QString sqlcmd = "Select Mpath From Model Where Mname=\'"+name+"\'";
		QSqlQuery query = db.exec(sqlcmd);
		if (query.next())
		{
			return query.value(0).toString();
		}
		else return "";
	}
    //treename--->modelpath
    QString FindPatyByName(QString name)
    {
        db = QSqlDatabase::database("test", false);
        QString sqlcmd = "Select Mpath From Model Where Tname=\'"+name+"\'";
        QSqlQuery query = db.exec(sqlcmd);
        if (query.next())
        {
            return query.value(0).toString();
        }
        else return "";
    }
    //treename - > modelname
    QString FindMnameByTreeName(QString name)
    {
        db = QSqlDatabase::database("test", false);
        QString sqlcmd = "Select Mname From Model Where Tname=\'"+name+"\'";
        QSqlQuery query = db.exec(sqlcmd);
        if (query.next())
        {
            return query.value(0).toString();
        }
        else return "";
    }


    //Modelname---THpath
    QString FindTHpathByMname(QString name)
    {
        db = QSqlDatabase::database("test", false);
        QString thp = "Select THpath From Model Where Mname=\'"+name+"\'";
        QSqlQuery query = db.exec(thp);
            if (query.next())
            {
                return query.value(0).toString();
            }
            else
                return "";
    }
    //Modelname-->Treename
    QString FindTnameByMname(QString name)
    {
        db = QSqlDatabase::database("test", false);
        QString tn = "Select Tname From Model Where Mname=\'"+name+"\'";
        QSqlQuery query = db.exec(tn);
            if (query.next())
            {
                return query.value(0).toString();
            }
            else
                return "";
    }

    QSqlTableModel* infomodel(QString tname)
    {
        db = QSqlDatabase::database("test", false);
        QSqlTableModel* im = new QSqlTableModel(0,db);
        im->setTable("Trees");
        im->setFilter(QObject::tr("Tname = '%1'").arg(tname));
        im->select();

        return im;
    }


	~DBManager()
	{
		db.close();
	}
};

#endif
