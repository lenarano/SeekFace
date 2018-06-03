#ifndef SUPERVISION_H
#define SUPERVISION_H

#include "Camera.h"
#include "DataBaseWorker.h"
#include "DataBaseVisitor.h"
#include "InfoBrowser.h"
#include "Users.h"

#include <QtGui>



class Supervision : public QMainWindow
{
	Q_OBJECT

public:
	Supervision(QWidget* pwgt = 0);
	
	
	private slots:
	void slotAbout();
	void slotHelp();
	void PushWorker();
	void faceSearch(QVector<float> my128Face);
	void LabelZero(QPixmap);
	void PushVisiter();

private:
	
	QMenuBar *menuBar;
	bool samePeople;
	int countFace;
	int countNotFace;
	QLabel *FixFace;
	QLabel *IDFace;
	QScrollArea *scrollCamera;
	QScrollArea *scrollFixFace;
	QScrollArea *scrollIDFace;
	QVBoxLayout *Vlayout1;
	QHBoxLayout *Hlayout1;
	QHBoxLayout *Hlayout2;
	QVBoxLayout *Hlayout2_butIDFace;
	QHBoxLayout *mainLayout;
	QWidget *mainWidget;

	Camera w;
	DataBaseWorker *Work;
	DataBaseVisitor *Visit;
	Users *access;
	InfoBrowser *helpBrowser;
	DataBase        *DB;

	QMenu *DBFaceMenu;
	QAction *openIDFace;
	QAction *openViziter;
	QMenu *helpMenu;
	QAction *аboutAct;
	QAction *helpAct;
	QMenu *exitMenu;
	QAction *exitAct;
	QString RUS(const char* t) { return QString::fromLocal8Bit(t); }

	QLabel *labelMenu;
	QLabel *labelFile;
	//Идентифицированное лицо
	QLineEdit *nameLine;
	QLineEdit *departmentLine;
	QLineEdit *positionLine;
	QLineEdit *EmailLine;
	QLineEdit *PhoneLine;
	//Выявленное лицо
	QLineEdit *accessDateLine;
	QLineEdit *accessTimeLine;
	QLineEdit *idLine;
	int id;

	void createForm();
	void createActions();
	void createMenus();
};

#endif