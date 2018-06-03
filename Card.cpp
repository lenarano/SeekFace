#include "Card.h"


Card::Card(QWidget* parent/*= 0*/)
	: QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint), ifIDFoto(false)
{
	createActions();
	createMenus();
	createForm();
	
}

QMenuBar * Card::getMyBar() { return myBar; }

QImage Card::getImg() { return img; }

QLineEdit * Card::getNameLine() { return nameLine; }

QLineEdit * Card::getDepartmentLine() { return departmentLine; }

QLineEdit * Card::getPositionLine() { return positionLine; }

QLineEdit * Card::getEmailLine() { return emailLine; }

QLineEdit * Card::getPhoneLine() { return phoneLine; }

bool  Card::getIfIDFoto() { return ifIDFoto; }

void Card:: createForm()
{
	QGroupBox *groupBox = new QGroupBox(RUS("Карточка сотрудника"));

	IDFace = new QLabel;
	IDFace->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	IDFace->setScaledContents(true);

	scrollIDFace = new QScrollArea;
	scrollIDFace->setBackgroundRole(QPalette::Dark);
	scrollIDFace->setWidget(IDFace);

	QLabel *name = new QLabel(RUS("ФИО:"));
	nameLine = new QLineEdit;
	QLabel *department = new QLabel(RUS("Отдел:"));
	departmentLine = new QLineEdit;
	QLabel *position = new QLabel(RUS("Должность:"));
	positionLine = new QLineEdit;
	QLabel *Email = new QLabel(RUS("E-mail:"));
	emailLine = new QLineEdit;
	QLabel *Phone = new QLabel(RUS("Телефон:"));
	phoneLine = new QLineEdit;

	QBoxLayout* MyLabel = new QVBoxLayout;
	MyLabel->addWidget(name);
	MyLabel->addWidget(department);
	MyLabel->addWidget(position);
	MyLabel->addWidget(Email);
	MyLabel->addWidget(Phone);

	QBoxLayout* MyLineEdit = new QVBoxLayout;
	MyLineEdit->addWidget(nameLine);
	MyLineEdit->addWidget(departmentLine);
	MyLineEdit->addWidget(positionLine);
	MyLineEdit->addWidget(emailLine);
	MyLineEdit->addWidget(phoneLine);

	QBoxLayout* MyLabelLineEdit = new QHBoxLayout;
	MyLabelLineEdit->addLayout(MyLabel);
	MyLabelLineEdit->addLayout(MyLineEdit);
	MyLabelLineEdit->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

	QBoxLayout* Mylayout = new QHBoxLayout;
	Mylayout->addWidget(scrollIDFace);
	Mylayout->addLayout(MyLabelLineEdit);

	groupBox->setLayout(Mylayout);

	QPushButton* okBtn = new QPushButton(RUS("Сохранить"));
	connect(okBtn, SIGNAL(clicked()), SLOT(accept()));

	QPushButton* cancelBtn = new QPushButton(RUS("Выход"));
	connect(cancelBtn, SIGNAL(clicked()), SLOT(reject()));

	QBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(okBtn);
	layout->addWidget(cancelBtn);

	QBoxLayout* layoutCard = new QVBoxLayout;
	layoutCard->addWidget(groupBox);
	layoutCard->addLayout(layout);
	setLayout(layoutCard);
}

void Card::createActions()
{
	openAct = new QAction(RUS("О&ткрыть..."), this);
	openAct->setIcon(QPixmap("./Resources/image/openFile.png"));
	openAct->setStatusTip(RUS("Загрузить изображение"));
	openAct->setShortcut(tr("Ctrl+O"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	cleanAct = new QAction(RUS("Очистить..."), this);
	cleanAct->setIcon(QPixmap("./Resources/image/clearCard.png"));
	cleanAct->setStatusTip(RUS("Очистить некорректно заполненную карточку"));
	connect(cleanAct, SIGNAL(triggered()), this, SLOT(slotСlean()));

	exitAct = new QAction(RUS("В&ыход"), this);
	exitAct->setIcon(QPixmap("./Resources/image/exit.png"));
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	zoomInAct = new QAction(RUS("Увеличение исходного изображения"), this);
	zoomInAct->setEnabled(false);
	connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

	zoomOutAct = new QAction(RUS("Уменьшение исходного изображения"), this);
	zoomOutAct->setEnabled(false);
	connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

	normalSizeAct = new QAction(RUS("Нормальный размер исходного изображения"), this);
	normalSizeAct->setEnabled(false);
	connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

	fitToWindowAct = new QAction(RUS("Растянуть исходное изображение"), this);
	fitToWindowAct->setEnabled(false);
	fitToWindowAct->setCheckable(true);
	connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));
}

void Card::createMenus()
{
	fileMenu = new QMenu(RUS("Файл"));
	fileMenu->addAction(openAct);
	fileMenu->addAction(cleanAct);
	
	viewMenu = new QMenu(RUS("Камера"), this);
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
	viewMenu->addAction(normalSizeAct);
	viewMenu->addSeparator();
	viewMenu->addAction(fitToWindowAct);

	exitMenu = new QMenu(RUS("Выход"));
	exitMenu->addAction(exitAct);

	myBar = new QMenuBar(this);
	myBar->addMenu(fileMenu);
	myBar->addMenu(viewMenu);
	myBar->addMenu(exitMenu);
		
}

void Card::updateActions()
{
	zoomInAct->setEnabled(!fitToWindowAct->isChecked());
	zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
	normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void Card::scaleImage(double factor)
{
	Q_ASSERT(IDFace->pixmap());
	scaleFactor *= factor;
	IDFace->resize(scaleFactor * IDFace->pixmap()->size());

	adjustScrollBar(scrollIDFace->horizontalScrollBar(), factor);
	adjustScrollBar(scrollIDFace->verticalScrollBar(), factor);

	zoomInAct->setEnabled(scaleFactor < 3.0);
	zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void Card::zoomIn()
{
	scaleImage(1.25);
}

void Card::zoomOut()
{
	scaleImage(0.8);
}

void Card::normalSize()
{
	IDFace->adjustSize();
	scaleFactor = 1.0;
}

void Card::fitToWindow()
{
	bool fitToWindow = fitToWindowAct->isChecked();
	scrollIDFace->setWidgetResizable(fitToWindow);
	if (!fitToWindow) {
		normalSize();
	}
	updateActions();
}

void Card::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
	scrollBar->setValue(int(factor * scrollBar->value()
		+ ((factor - 1) * scrollBar->pageStep() / 2)));
}


void Card::open()
{
	fileName = QFileDialog::getOpenFileName(this, RUS("Загрузить фотографию сотрудника"), QDir::currentPath());
	if (!fileName.isEmpty()) {
		QImage image(fileName);
		if (image.isNull()) {
			QMessageBox::information(this, tr("Ошибка загрузки"),
				tr("Cannot load %1.").arg(fileName));
			return;
		}
		IDFace->setPixmap(QPixmap::fromImage(image));
		img = QPixmap::fromImage(image).toImage();// преобразовала QPixmap  в QImage
		scaleFactor = 1.0;
		fitToWindowAct->setEnabled(true);
		updateActions();
		if (!fitToWindowAct->isChecked())
			IDFace->adjustSize();
		ifIDFoto = true;
	}
}



QString Card::getInput(QLineEdit* m_edit) const {
	return m_edit->text();
}


void Card::slotСlean()
{
	nameLine->setText(RUS(""));
	departmentLine->setText(RUS(""));
	positionLine->setText(RUS(""));
	emailLine->setText(RUS(""));
	phoneLine->setText(RUS(""));
	QPixmap pix = QPixmap("./Resources/image/zero.png");
	IDFace->setPixmap(pix.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	IDFace->adjustSize();
}

