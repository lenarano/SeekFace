#include "Camera.h"

Camera::Camera(QWidget* parent)
	: QWidget(parent), cap(0), count(0), IfFace(false) {
	if (!cap.isOpened())  //  Проверка корректности отработки
	{
		QMessageBox::critical(0,"Attention","Check the camera connection!",QMessageBox::Close);
		//завершение работы
	}
	
	labelCamera = new QLabel;
	labelCamera->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	labelCamera->setScaledContents(true);

	labelFace = new QLabel;
	labelFace->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	labelFace->setScaledContents(true);

	connect(&m_timer, SIGNAL(timeout()), SLOT(setCamera()));
	m_timer.start(20);

	//создала поток и класс для поиска лица на картинке
	thread = new QThread;
	Detector = new FindFace("detectorFace");
	Detector->moveToThread(thread);
	connect(this, SIGNAL(sendGlobalVar(cv::Mat)), Detector, SLOT(detectorFace(cv::Mat)));
	thread->start();
}

Camera::~Camera() 
{

}

QString Camera::getDate() { return date;}

QString Camera::getTime() { return time;}

void Camera::setIfFace(bool face) {IfFace= face;}

QPixmap Camera::getPix() { return pix;}

QLabel* Camera::getLabelCamera() { return labelCamera;}

QLabel* Camera::getLabelFace() { return labelFace; }

cv::VideoCapture Camera::getCap() { return cap; }

void Camera::setCamera() {
	
	cap >> image;
	count++;
	if ((count % SKIP_FRAMES == 0) && (IfFace == false))//каждую секунду считываем изображение
	{
		count = 0;
		emit sendGlobalVar(image);
		if (Detector->getfaces().size() == 1)//если есть человек
		{
			IfFace = true;
			//проверка, чтобы не вылазило за границы
			bool is_inside = (Detector->getface() & cv::Rect(0, 0, image.cols, image.rows)) == Detector->getface();
			if (is_inside)
			{
				//для отображения зафиксированного лица
				DetectorFaceImg = image(Detector->getface()).clone();
				setLabel(DetectorFaceImg, labelFace);
				date = QDate::currentDate().toString("dd.MM.yyyy");
				time = QTime::currentTime().toString("hh:mm:ss");
				dlib::matrix<float, 0, 1> face128 = Detector->descriptorFace1();//получаем дискриптор лица
				FaceFix128 = convertDlibToStdVector(face128);
				applied(FaceFix128);//сигнал модулю управления
			}
			else
			{
				qDebug() << "The resulting face goes beyond the camera capture: " << is_inside;
				IfFace = false;
			}
		}
		else if (Detector->getfaces().size() == 0)
		{
			QPixmap pix = QPixmap("./Resources/image/zero.png");
			labelFace->setPixmap(pix);
			labelFace->adjustSize();
			ifLabelZero(pix);
		}
		else if (Detector->getfaces().size() > 1)
		{
			QPixmap pix = QPixmap("./Resources/image/Faces.jpg");
			labelFace->setPixmap(pix);
			labelFace->adjustSize();
			ifLabelZero(pix);
		}

	}
	//для отображения камеры в виджете
	setLabel(image, labelCamera);
}

QVector<float> Camera::convertDlibToStdVector(dlib::matrix<float, 0, 1> face128)
{
	QVector<float> x;
	for (unsigned int r = 0; r < face128.nr(); r += 1)
	{
		x.push_back(face128(r, 0));
	}
	return x;
}

void Camera::setLabel(cv::Mat image, QLabel * label)
{
	cv::Mat dest;
	cvtColor(image, dest, CV_BGR2RGB);
	QImage img = QImage((uchar*)dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888);
	pix = QPixmap::fromImage(img);
	label->setPixmap(pix);
	label->adjustSize();
}







