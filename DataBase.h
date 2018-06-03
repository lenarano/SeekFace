#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QTextCodec>
#include <QMap>
#include <QImage>
#include <QBuffer>
#include <QPixmap>
#include <QMessageBox>

#include <iostream>
#include <iomanip>


//Директивы имен таблицы, полей таблицы и базы данных 
#define DATABASE_HOSTNAME   "FaceDataBase"
#define DATABASE_NAME       "SeekFace.db"
//БД сотрудников
#define TABLE_WORKER            "WorkerTable"     
#define TABLE_NAME              "TimeWorker"              
#define TABLE_PIC_WORKER        "PicWork"             
#define TABLE_DECKRIPTOR        "Deckriptor"
#define TABLE_NAME_RUS          "NameRus"
#define TABLE_DEPARTMENT        "Department"
#define TABLE_POSITION          "Position"
#define TABLE_PHONE             "Phone"
#define TABLE_EMAIL             "Email"
//БД посетителей
#define TABLE_VISITOR           "VisitorTable"
#define TABLE_PIC_VISITOR       "PicVisit"
#define TABLE_INDEF             "Indef"
#define TABLE_DATE              "Date"
#define TABLE_TIME              "Time"
//БД паролей
#define TABLE_USERS             "UsersTable"
#define TABLE_USERNAME          "Username"
#define TABLE_PASSWORD          "Password"
#define TABLE_ROLE              "Role"


class DataBase : public QObject
{
	Q_OBJECT
public:
	explicit DataBase(QObject *parent = 0);
	~DataBase();
	
	bool  getTruePassword();
	QPixmap  getImageFoto();
	QString getSurname();
	QString getDepartment();
	QString getPosition();
	QString getPhone();
	QString getEmail();
	void connectToDataBase();
	QMap<int, QVector<float>> selectFromTable();// Выборка
	void selectFromTableIDFace(int idFace);
	void passwordVerification(QString username, QString password);

	public slots:
	bool insertIntoWorkerTable(const QVariantList &data);      // Добавление записей в таблицу
	bool insertIntoVisitorTable(const QVariantList &data);
	bool insertIntoUsersTable(const QVariantList &data);
	bool insertIntoTable(const QByteArray &pic, const QByteArray &deck, const QString &rusName, const QString &depart, const QString &pos, const QString &phone, const QString &email, const QString &date);
	bool insertIntoTable(const QPixmap &pix, const QString &Identif, const QString &Date, const QString &Time);
	bool insertIntoTable(const QString &name, const QString &pass, const QString &role);

private:
	QSqlDatabase    db;
	QPixmap imageFoto;
	QString surname;
	QString department;
	QString position;
	QString phone;
	QString email;
	bool truePassword;
	int Id;
	bool openDataBase();      
	bool restoreDataBase();   
	void closeDataBase();       
	bool createVisitorTable();         
	bool createWorkerTable();
	bool createUsersTable();
	QString RUS(const char* t) { return QString::fromLocal8Bit(t); }

	
};

#endif // DATABASE_H
