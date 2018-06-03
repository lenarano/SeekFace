#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QTableView>
#include <QPushButton>
#include <QHeaderView>
#include <QMenuBar>
#include <QApplication>
#include <QScreen>
#include <QStatusBar>
#include <QVector>

#include <iostream>
#include <dlib/matrix.h>

#include "DataBase.h"
#include "Card.h"
#include "FindFace.h"


class DataBaseWorker : public QMainWindow
{
	Q_OBJECT

public:
	DataBaseWorker(DataBase  *db);
	~DataBaseWorker();
	
	DataBase  * getDb();
	QString getIDname();
	QString getIDdepartment();
	QString getIDposition();
	QString getIDEmail();
	QString getIDPhone();
	int getIdRezulFace();
	QMenuBar* getMenuBar();
	double faceSearchDB(QVector<float> my128Face);
	void selectIDInDB();

	private slots:
	void saveWorker();
	void deleteWorker();
	void slotCurrentPic(QModelIndex index);

private:
	DataBase  *db;
	int idRezulFace;//номер ключа для дальнейшей выборки в БД
	QImage IDFoto;
	QString IDname;
	QString IDdepartment;
	QString IDposition;
	QString IDEmail;
	QString IDPhone;
	cv::Mat matFoto;
	QMenuBar *menuBar;
	QLabel* LabelImg;
	QTableView *tableView;
	QMenu *workerMenu;
	QAction *openAct;
	QAction *deleteAct;
	QMenu *exitMenu;
	QAction *exitAct;
	QSqlTableModel *modelWorker;
	FindFace *FaceDesk;
	QWidget *mainWidget;
	QLabel *labelMenu;
	QLabel *labelFile;
	QMap<int, QVector<float>> dataFases;
	void setupModel(const QString &tableName, const QStringList &headers);
	void createForm();
	void createActions();
	void createMenus();
	void createUI();
	QString RUS(const char* t) { return QString::fromLocal8Bit(t); }
	cv::Mat qimage2mat(const QImage& qimage);
	QVector<float> convertDlimToStdVector(dlib::matrix<float, 0, 1> face128);
	void printVector(QVector<float> my128Face);
};

#endif  