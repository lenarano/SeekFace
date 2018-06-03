#ifndef CAMERA_H
#define CAMERA_H

#define SKIP_FRAMES 20//детектор лица каждую секунду


#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QThread>
#include <QVector>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "FindFace.h"
#include <iostream>
#include <dlib/matrix.h>




class Camera : public QWidget {
	Q_OBJECT

public:
	Camera(QWidget* parent = 0);
	~Camera();
	
	QLabel* getLabelFace();
	QLabel* getLabelCamera();
	QPixmap getPix();
	void setIfFace(bool face);
	QString getDate();
	QString getTime();
	cv::VideoCapture getCap();
	void setLabel(cv::Mat image, QLabel * label);


signals:
	void sendGlobalVar(cv::Mat);
	void applied(QVector<float>);
	void ifLabelZero(QPixmap);

	private slots:
	void setCamera();

private:
	QLabel *labelFace;
	QLabel *labelCamera;
	QVector<float> FaceFix128;
	QPixmap pix;
	bool IfFace;
	QString date;
	QString time;
	QTimer m_timer;
	cv::Mat image;
	cv::Mat DetectorFaceImg;
	cv::VideoCapture cap;
	QThread *thread;
	FindFace *Detector;
	int count;
	QString RUS(const char* t) { return QString::fromLocal8Bit(t); }
	QVector<float> convertDlibToStdVector(dlib::matrix<float, 0, 1> face128);
	

};

#endif
