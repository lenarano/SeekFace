#include "DataBase.h"

DataBase::DataBase(QObject *parent) : QObject(parent), truePassword(false)
{


}

DataBase::~DataBase()
{
}


QPixmap  DataBase::getImageFoto() { return imageFoto; }

QString DataBase::getSurname() { return surname; }

QString DataBase::getDepartment() { return department; }

QString DataBase::getPosition() { return position; }

QString DataBase::getPhone() { return phone; }

QString DataBase::getEmail() { return email; }

bool  DataBase::getTruePassword() { return truePassword; }

//Методы для подключения к базе данных
void DataBase::connectToDataBase()
{
	/* Перед подключением к базе данных производим проверку на её существование.
	* В зависимости от результата производим открытие базы данных или её восстановление
	* */
	if (!QFile("./Resources/database/" DATABASE_NAME).exists()) {
		this->restoreDataBase();
	}
	else {
		this->openDataBase();
	}
}

/* Методы восстановления базы данных
* */
bool DataBase::restoreDataBase()
{
	if (this->openDataBase()) {
		if ((!this->createVisitorTable()) || (!this->createWorkerTable()) || (!this - createUsersTable())) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		qDebug() << "Не удалось восстановить базу данных";
		return false;
	}
	return false;

}

/* Метод для открытия базы данных
* */
bool DataBase::openDataBase()
{
	if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
		db = QSqlDatabase::database();
	}
	else {
		db = QSqlDatabase::addDatabase("QSQLITE");
		db.setHostName(DATABASE_HOSTNAME);
		db.setDatabaseName("./Resources/database/" DATABASE_NAME);
	}

	if (!db.open()) {
		qDebug() << "Cannot open database:" << db.lastError();
		return false;
	}
	return true;

}

/* Методы закрытия базы данных
* */
void DataBase::closeDataBase()
{
	db.close();
}

/* Метод для создания таблицы в базе данных
* */
bool DataBase::createVisitorTable()
{
	QSqlQuery query;
	if (!query.exec("CREATE TABLE " TABLE_VISITOR " ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		TABLE_PIC_VISITOR  " BLOB            NOT NULL,"
		TABLE_INDEF        " VARCHAR(255)    NOT NULL,"
		TABLE_DATE         " VARCHAR(255)    NOT NULL,"
		TABLE_TIME         " VARCHAR(255)    NOT NULL "
		" )"
	)) {
		qDebug() << "DataBase: error of create " << TABLE_VISITOR;
		qDebug() << query.lastError().text();
		return false;
	}
	else {
		return true;
	}
	return false;
}


bool DataBase::createWorkerTable()
{
	/* В данном случае используется формирование сырого SQL-запроса
	* с последующим его выполнением.
	* */
	QSqlQuery query;
	if (!query.exec("CREATE TABLE " TABLE_WORKER " ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		TABLE_PIC_WORKER " BLOB            NOT NULL,"
		TABLE_DECKRIPTOR " BLOB            NOT NULL,"
		TABLE_NAME_RUS   " VARCHAR(255)    NOT NULL,"
		TABLE_DEPARTMENT " VARCHAR(255)    NOT NULL,"
		TABLE_POSITION   " VARCHAR(255)    NOT NULL,"
		TABLE_PHONE      " VARCHAR(255)    NOT NULL,"
		TABLE_EMAIL      " VARCHAR(255)    NOT NULL,"
		TABLE_NAME       " VARCHAR(255)    NOT NULL"
		" )"
	)) {
		qDebug() << "DataBase: error of create " << TABLE_WORKER;
		qDebug() << query.lastError().text();
		return false;
	}
	else {
		return true;
	}
	return false;
}

bool DataBase::createUsersTable()
{
	QSqlQuery query;
	if (!query.exec("CREATE TABLE " TABLE_USERS " ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		TABLE_USERNAME   " VARCHAR(255)    NOT NULL,"
		TABLE_PASSWORD   " VARCHAR(255)    NOT NULL,"
		TABLE_ROLE       " VARCHAR(255)    NOT NULL"
		" )"
	)) {
		qDebug() << "DataBase: error of create " << TABLE_USERS;
		qDebug() << query.lastError().text();
		return false;
	}
	else {
		insertIntoTable("1111", "1111", "-");
		return true;
	}
	return false;
}

bool DataBase::insertIntoTable(const QString &name, const QString &pass, const QString &role)
{
	QVariantList data;

	data.append(name);
	data.append(pass);
	data.append(role); 

	if (insertIntoUsersTable(data))
		return true;
	else
		return false;
}

bool DataBase::insertIntoUsersTable(const QVariantList &data)
{
	QSqlQuery query;
	query.prepare("INSERT INTO " TABLE_USERS " ( " TABLE_USERNAME ", "
		TABLE_PASSWORD ", "
		TABLE_ROLE " ) "
		"VALUES (:Name,:Pass,:Role)");
	query.bindValue(":Name", data[0].toString());
	query.bindValue(":Pass", data[1].toString());
	query.bindValue(":Role", data[2].toString());
	// После чего выполняется запросом методом exec()
	if (!query.exec()) {
		qDebug() << "error insert into " << TABLE_USERS;
		qDebug() << query.lastError().text();
		return false;
	}
	else {
		return true;
	}
	return false;
}


//Метод для вставки записи в базу данных
bool DataBase::insertIntoWorkerTable(const QVariantList &data)
{
	/* Запрос SQL формируется из QVariantList,
	* в который передаются данные для вставки в таблицу.
	* */
	QSqlQuery query;
	/* В начале SQL запрос формируется с ключами,
	* которые потом связываются методом bindValue
	* для подстановки данных из QVariantList
	* */
	query.prepare("INSERT INTO " TABLE_WORKER " ( " TABLE_PIC_WORKER ", "
		TABLE_DECKRIPTOR ", "
		TABLE_NAME_RUS ", "
		TABLE_DEPARTMENT ", "
		TABLE_POSITION ", "
		TABLE_PHONE ", "
		TABLE_EMAIL ", "
		TABLE_NAME " ) "
		"VALUES (:Pic,:Deckr,:NameRUS, :Depart, :Posit, :Phone, :Email, :Name)");
	query.bindValue(":Pic", data[0].toByteArray());
	query.bindValue(":Deckr", data[1].toByteArray());
	query.bindValue(":NameRUS", data[2].toString());
	query.bindValue(":Depart", data[3].toString());
	query.bindValue(":Posit", data[4].toString());
	query.bindValue(":Phone", data[5].toString());
	query.bindValue(":Email", data[6].toString());
	query.bindValue(":Name", data[7].toString());
	// После чего выполняется запросом методом exec()
	if (!query.exec()) {
		qDebug() << "error insert into " << TABLE_WORKER;
		qDebug() << query.lastError().text();
		return false;
	}
	else {
		return true;
	}
	return false;
}


bool DataBase::insertIntoVisitorTable(const QVariantList &data)
{
	QSqlQuery query;
	query.prepare("INSERT INTO " TABLE_VISITOR " ( " TABLE_PIC_VISITOR ", "
		TABLE_INDEF ", "
		TABLE_DATE ", "
		TABLE_TIME " ) "
		"VALUES (:Pic,:Identif, :Date, :Time)");
	query.bindValue(":Pic", data[0].toByteArray());
	query.bindValue(":Identif", data[1].toString());
	query.bindValue(":Date", data[2].toString());
	query.bindValue(":Time", data[3].toString());
	// После чего выполняется запросом методом exec()
	if (!query.exec()) {
		qDebug() << "error insert into " << TABLE_VISITOR;
		qDebug() << query.lastError().text();
		return false;
	}
	else {
		return true;
	}
	return false;
}


bool DataBase::insertIntoTable(const QByteArray &pic, const QByteArray &deck, const QString &rusName, const QString &depart, const QString &pos, const QString &phone, const QString &email, const QString &date)
{
	QVariantList data;

	data.append(pic);
	data.append(deck);
	data.append(rusName); // ФИО
	data.append(depart); // Отдел
	data.append(pos); // Отдел
	data.append(phone); // Телефон
	data.append(email); // Электронный адрес
	data.append(date); //Дата сохранения

	if (insertIntoWorkerTable(data))
		return true;
	else
		return false;
}

bool DataBase::insertIntoTable(const QPixmap &pix, const QString &Identif, const QString &Date, const QString &Time)
{
	//запись в БД фото
	QByteArray inByteArray;// Создаём объект QByteArray для сохранения изображения;                             
	QBuffer inBuffer(&inByteArray);// Сохранение изображения производим через буффер
	inBuffer.open(QIODevice::WriteOnly);// Открываем буффер
	pix.save(&inBuffer, "PNG");// Записываем inPixmap в inByteArray

	QVariantList data;
	data.append(inByteArray);
	data.append(Identif);
	data.append(Date); // ФИО
	data.append(Time); // Отдел
	
	if (insertIntoVisitorTable(data))
		return true;
	else
		return false;
}


//Метод для вставки записи в базу данных
QMap<int, QVector<float>> DataBase::selectFromTable()
{
	QSqlQuery query;
	if (!query.exec("SELECT id, Deckriptor FROM " TABLE_WORKER ";")) {
		qDebug() << "Unable to execute query - exiting SELECT";
	}
	//Считываем значения
	QSqlRecord rec = query.record();
	int        nNumber = 0;
	QByteArray desk;
	QVector<float> data(128);
	QMap<int, QVector<float>> mapFases;
	while (query.next()) {
		nNumber = query.value(rec.indexOf("id")).toInt();
		desk = query.value(rec.indexOf("Deckriptor")).toByteArray();
		//преобразуем обратно в vector
		QByteArray dou(sizeof(float), 0x0);
		for (int i = 0; i < data.count(); i++) {
			dou = desk.mid(i * sizeof(float), sizeof(float));
			data[i] = *reinterpret_cast<const float*>(dou.data());
			dou.clear();
		}
		//printVector(data);
		mapFases[nNumber] = data;
	}
	return mapFases;
}


void DataBase::selectFromTableIDFace(int idFace)
{
	QSqlQuery query;
	QString queryint = "Select id, PicWork, NameRus, Department, Position, Phone, Email from " TABLE_WORKER " where id = '%1'";
	if (!query.exec(queryint.arg(idFace)))
	{
		qDebug() << "Unable to execute query - exiting SELECT id";
	}
	//Считываем значения
	QSqlRecord rec = query.record();
	while (query.next()) {
		QByteArray PicFoto;
		Id = query.value(rec.indexOf("id")).toInt();
		PicFoto = query.value(rec.indexOf("PicWork")).toByteArray();
		surname = query.value(rec.indexOf("NameRus")).toString();
		department = query.value(rec.indexOf("Department")).toString();
		position = query.value(rec.indexOf("Position")).toString();
		phone = query.value(rec.indexOf("Phone")).toString();
		email = query.value(rec.indexOf("Email")).toString();
		//преобразуем QByteArray обратно в QImage imageFoto
		imageFoto = QPixmap();
		imageFoto.loadFromData(PicFoto, "PNG");
	}
}

void DataBase::passwordVerification(QString username, QString password)
{
	QSqlQuery qry;
	if (qry.exec("SELECT Username, Password, Role FROM UsersTable WHERE Username=\'" + username +
		"\' AND Password=\'" + password + "\'"))
	{
		if (qry.next()) {
			truePassword = true;//правильно
		}
		else {//неверно
			QMessageBox::information(0, RUS("Внимание:"), RUS("Неверное Имя пользователя или пароль!"));
			truePassword = false;
		}
	}
	else {
		qDebug() << "Wrong query";
	}
}
