#ifndef DATABASEVISITOR_H
#define DATABASEVISITOR_H

#include <QMainWindow>
#include <QGroupBox>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QPixmap>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QHeaderView>
#include <QMenuBar>
#include <QApplication>
#include <QBuffer>
#include <QScreen>
#include <QStatusBar>

#include <iostream>
#include <iomanip>


#include "DataBase.h"
#include "tablemodel.h"


class DataBaseVisitor : public QMainWindow
{
	Q_OBJECT

public:
	DataBaseVisitor(DataBase  *db);
	~DataBaseVisitor();
	
	TableModel* getModelVisitor();
	QMenuBar* getMenuBar();
		
	private slots:
	void deleteVisitor();

private:
	DataBase  *db;
	QMenuBar *menuBar;
	// Сам объект базы данных, с которым будет производиться работа
	TableModel  *modelVisitor;
	QLabel *labelMenu;
	QLabel *labelFile;
	QAction *deleteAct;
	QAction *exitAct;
	QMenu *visitorMenu;
	QMenu *exitMenu;
	QGroupBox *groupBox;
	QVBoxLayout* layoutLabel;
	QTableView *tableView;
	QWidget *mainWidget;
	void setupModel(const QString &tableName, const QStringList &headers);
	void createForm();
	void createActions();
	void createMenus();
	void createUI();
	QString RUS(const char* t) { return QString::fromLocal8Bit(t); }
	
};

#endif // DATABASE_H

