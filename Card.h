#ifndef CARD_H
#define CARD_H

#include <QDialog>
#include <QGroupBox>
#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QScrollBar>



class Card : public QDialog {
	Q_OBJECT

public:
	Card(QWidget* parent = 0);
	
	QMenuBar * getMyBar();
	QImage getImg();
	bool  getIfIDFoto();
	QLineEdit * getNameLine();
	QLineEdit * getDepartmentLine();
	QLineEdit * getPositionLine();
	QLineEdit * getEmailLine();
	QLineEdit * getPhoneLine();
	
	QString getInput(QLineEdit* m_edit) const;

	private slots:
	void open();
	void slot—lean();
	void fitToWindow();
	void normalSize();
	void zoomOut();
	void zoomIn();

private:
	QMenuBar *myBar;
	QImage img;
	QLineEdit *nameLine;
	QLineEdit *departmentLine;
	QLineEdit *positionLine;
	QLineEdit *emailLine;
	QLineEdit *phoneLine;
	QLabel *IDFace;
	QString fileName;
	bool ifIDFoto;
	QAction *openAct;
	QAction *exitAct;
	QAction *cleanAct;
	QMenu *fileMenu;
	QMenu *exitMenu;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *normalSizeAct;
	QAction *fitToWindowAct;
	QMenu *viewMenu;
	QScrollArea *scrollIDFace;
	double scaleFactor;
	void createForm();
	void createActions();
	void createMenus();
	QString RUS(const char* t) { return QString::fromLocal8Bit(t); }
	void updateActions();
	void scaleImage(double factor);
	void adjustScrollBar(QScrollBar *scrollBar, double factor);

};
#endif  

