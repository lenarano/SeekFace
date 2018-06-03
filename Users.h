#ifndef USERS_H
#define USERS_H


#include <QDialog>
#include <QGroupBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QScrollBar>



class Users : public QDialog {
	Q_OBJECT

public:
	Users(QWidget* parent = 0);
	QHBoxLayout* getLayout();
	QLineEdit * getUsernameLine();
	QLineEdit * getPasswordLine();

	private slots:
	void clearForm();

private:
	QString RUS(const char* t) { return QString::fromLocal8Bit(t); }
	QHBoxLayout* layout;
	QLineEdit *usernameLine;
	QLineEdit *passwordLine;
};
#endif  

