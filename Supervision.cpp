#include "Supervision.h"




Supervision::Supervision(QWidget* pwgt) : QMainWindow(pwgt), id(0), samePeople(false), countFace(0), countNotFace(0)
{
	DB = new DataBase();
	DB->connectToDataBase();

	Work = new DataBaseWorker(DB);
	Visit = new DataBaseVisitor(DB);
	connect(&w, SIGNAL(applied(QVector<float>)), SLOT(faceSearch(QVector<float>)));
	connect(&w, SIGNAL(ifLabelZero(QPixmap)), SLOT(LabelZero(QPixmap)));
	//нарисовали форму
	createForm();
	createActions();
	createMenus();
	setWindowTitle(RUS("SeekFace"));
	setWindowIcon(QIcon("./Resources/image/icon.png"));
	resize(1000, 600);
}

void Supervision::createForm()
{
	FixFace = new QLabel;
	FixFace->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	FixFace->setScaledContents(true);

	IDFace = new QLabel;
	IDFace->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	IDFace->setScaledContents(true);

	scrollCamera = new QScrollArea;
	scrollCamera->setBackgroundRole(QPalette::Dark);
	scrollCamera->setWidget(w.getLabelCamera());
	scrollCamera->setWidgetResizable(true);

	scrollFixFace = new QScrollArea;
	scrollFixFace->setBackgroundRole(QPalette::Dark);
	scrollFixFace->setWidget(w.getLabelFace());
	scrollFixFace->setWidgetResizable(true);

	scrollIDFace = new QScrollArea;
	scrollIDFace->setBackgroundRole(QPalette::Dark);
	scrollIDFace->setWidget(IDFace);
	scrollIDFace->setWidgetResizable(true);

	Vlayout1 = new QVBoxLayout(this);
	Vlayout1->addWidget(scrollCamera);

	QGroupBox *camera = new QGroupBox(RUS("Камера"));
	camera->setLayout(Vlayout1);

	QLabel *accessDate = new QLabel(RUS("   Дата прохода:"));
	accessDateLine = new QLineEdit;
	QLabel *accessTime = new QLabel(RUS("   Время прохода:"));
	accessTimeLine = new QLineEdit;
	QLabel *id = new QLabel(RUS("   Идентифицирован:"));
	idLine = new QLineEdit;

	QGridLayout *visitor = new QGridLayout;
	visitor->addWidget(accessDate, 0, 0);
	visitor->addWidget(accessDateLine, 0, 1);
	visitor->addWidget(accessTime, 1, 0);
	visitor->addWidget(accessTimeLine, 1, 1);
	visitor->addWidget(id, 2, 0);
	visitor->addWidget(idLine, 2, 1);
	visitor->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

	Hlayout1 = new QHBoxLayout(this);
	Hlayout1->addWidget(scrollFixFace);
	Hlayout1->addLayout(visitor);

	QGroupBox *visitorGroupBox = new QGroupBox(RUS("Выявленное лицо"));
	visitorGroupBox->setLayout(Hlayout1);

	QLabel *name = new QLabel(RUS(" Фамилия Имя:"));
	nameLine = new QLineEdit;
	QLabel *department = new QLabel(RUS(" Отдел:"));
	departmentLine = new QLineEdit;
	QLabel *position = new QLabel(RUS(" Должность:"));
	positionLine = new QLineEdit;
	QLabel *Email = new QLabel(RUS(" Электронная почта:"));
	EmailLine = new QLineEdit;
	QLabel *Phone = new QLabel(RUS(" Телефон:"));
	PhoneLine = new QLineEdit;

	QGridLayout *worker = new QGridLayout;
	worker->addWidget(name, 0, 0);
	worker->addWidget(nameLine, 0, 1);
	worker->addWidget(department, 1, 0);
	worker->addWidget(departmentLine, 1, 1);
	worker->addWidget(position, 2, 0);
	worker->addWidget(positionLine, 2, 1);
	worker->addWidget(Email, 3, 0);
	worker->addWidget(EmailLine, 3, 1);
	worker->addWidget(Phone, 4, 0);
	worker->addWidget(PhoneLine, 4, 1);
	worker->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

	Hlayout2 = new QHBoxLayout(this);
	Hlayout2->addWidget(scrollIDFace);
	Hlayout2->addLayout(worker);

	QGroupBox *workerGroupBox = new QGroupBox(RUS("Результаты идентификации"));
	workerGroupBox->setLayout(Hlayout2);

	QVBoxLayout *Hlayout12 = new QVBoxLayout(this);
	Hlayout12->addWidget(visitorGroupBox);
	Hlayout12->addWidget(workerGroupBox);

	mainWidget = new QWidget();
	setCentralWidget(mainWidget);

	mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(camera);
	mainLayout->addLayout(Hlayout12);

	mainWidget->setLayout(mainLayout);
}

void Supervision::createActions()
{
	openIDFace = new QAction(RUS("БД сотрудников "), this);
	openIDFace->setIcon(QPixmap("./Resources/image/BDSotr.png"));
	openIDFace->setStatusTip(RUS("Загрузить базу данных сотрудников для просмотра и редактирования"));
	connect(openIDFace, SIGNAL(triggered()), this, SLOT(PushWorker()));

	openViziter = new QAction(RUS("БД посетителей"), this);
	openViziter->setIcon(QPixmap("./Resources/image/BDViz.png"));
	openViziter->setStatusTip(RUS("Загрузить базу данных посетителей для просмотра"));
	connect(openViziter, SIGNAL(triggered()), this, SLOT(PushVisiter()));

	аboutAct = new QAction(RUS("Информация о приложении"), this);
	аboutAct->setIcon(QPixmap("./Resources/image/Razr.png"));
	аboutAct->setStatusTip(RUS("Информация о приложении"));
	connect(аboutAct, SIGNAL(triggered()), this, SLOT(slotAbout()));

	helpAct = new QAction(RUS("Руководство пользователя"), this);
	helpAct->setIcon(QPixmap("./Resources/image/help.png"));
	helpAct->setStatusTip(RUS("Особенности работы с программой"));
	connect(helpAct, SIGNAL(triggered()), this, SLOT(slotHelp()));

	exitAct = new QAction(RUS("В&ыход"), this);
	exitAct->setIcon(QPixmap("./Resources/image/exit.png"));
	exitAct->setStatusTip(RUS("Завершить работу программы"));
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

}

void Supervision::createMenus()
{
	DBFaceMenu = new QMenu(RUS("Базы данных"));
	DBFaceMenu->addAction(openIDFace);
	DBFaceMenu->addAction(openViziter);
	helpMenu = new QMenu(RUS("Справка"));
	helpMenu->addAction(аboutAct);
	helpMenu->addAction(helpAct);
	exitMenu = new QMenu(RUS("Выход"));
	exitMenu->addAction(exitAct);
	menuBar = new QMenuBar(this);
	menuBar->addMenu(DBFaceMenu);
	menuBar->addMenu(helpMenu);
	menuBar->addMenu(exitMenu);
	layout()->setMenuBar(menuBar);

	labelMenu = new QLabel(statusBar());
	labelFile = new QLabel();

	statusBar()->setSizeGripEnabled(false);
	statusBar()->addWidget(labelMenu, 1);
	statusBar()->addWidget(labelFile, 2);
}


void Supervision::faceSearch(QVector<float> my128Face)
{
	accessDateLine->setText(w.getDate());
	accessTimeLine->setText(w.getTime());
	double evklid = Work->faceSearchDB(my128Face);//сравниваем с другими дескриптором и находим id наиболее подходящий
	if (evklid < 0.60)
	{
		countNotFace = 0;
		std::cout << " id " << id << " Work->idRezulFace " << Work->getIdRezulFace() << std::endl;
		if (id != Work->getIdRezulFace())//если появился другой человек
		{
			samePeople = true;//ставим ему пометку-разрешение пройти дальше
			countFace = 0;
			id = Work->getIdRezulFace();
		}
		if (id == Work->getIdRezulFace())
		{
			nameLine->setText(Work->getIDname());
			departmentLine->setText(Work->getIDdepartment());
			positionLine->setText(Work->getIDposition());
			EmailLine->setText(Work->getIDEmail());
			PhoneLine->setText(Work->getIDPhone());
			idLine->setText(Work->getIDname());
			IDFace->setPixmap(Work->getDb()->getImageFoto());// Устанавливаем изображение в QLabel
			IDFace->adjustSize();
		}

		if (id == Work->getIdRezulFace() && samePeople)//тотже и есть разрешение
		{
			countFace++;
			if (countFace == 2)//тотже
			{
				//std::cout << " Raspoznan " << countFace << std::endl;
				Work->selectIDInDB();//Делаем выборку наиболее подходящего
				DB->insertIntoTable(w.getPix(), Work->getIDname(), w.getDate(), w.getTime());
				Visit->getModelVisitor()->select();
				samePeople = false;
				countFace = 0;
				
			}
		}

	}
	else {
		id = 0;
		countFace = 0;
		samePeople = false;
		countNotFace++;
		nameLine->setText(RUS("НЕ РАСПОЗНАН"));
		departmentLine->setText(RUS("НЕ РАСПОЗНАН"));
		positionLine->setText(RUS("НЕ РАСПОЗНАН"));
		EmailLine->setText(RUS("НЕ РАСПОЗНАН"));
		PhoneLine->setText(RUS("НЕ РАСПОЗНАН"));
		idLine->setText(RUS("НЕ РАСПОЗНАН"));
		IDFace->setPixmap(QPixmap("./Resources/image/Razr.png"));// Устанавливаем изображение в QLabel
		IDFace->adjustSize();
		if (countNotFace == 2)
		{
			//std::cout << " Ne raspoznan " << countNotFace << std::endl;
			DB->insertIntoTable(w.getPix(), RUS("НЕ РАСПОЗНАН"), w.getDate(), w.getTime());
			//Visit->modelVisitor->select();
			countNotFace = 0;
		}
	}

	w.setIfFace(false);
}

void Supervision::LabelZero(QPixmap pix)
{
	nameLine->setText(RUS(" "));
	departmentLine->setText(RUS(" "));
	positionLine->setText(RUS(" "));
	EmailLine->setText(RUS(" "));
	PhoneLine->setText(RUS(" "));
	idLine->setText(RUS(" "));
	accessDateLine->setText(RUS(" "));
	accessTimeLine->setText(RUS(" "));
	IDFace->setPixmap(pix);// Устанавливаем изображение в QLabel
	IDFace->adjustSize();
}

void Supervision::PushWorker()
{
	access = new Users();
	access->setWindowTitle(RUS("Запрос пароля"));
	access->setLayout(access->getLayout());
	
	if (access->exec() == QDialog::Accepted)// Пользователь ввел пароль
	{
		QString Username = access->getUsernameLine()->text();
		QString Password = access->getPasswordLine()->text();
		DB->passwordVerification(Username, Password);
		
	}
	if (DB->getTruePassword() == true)
	{
		Work->setWindowTitle(RUS("База данных сотрудников"));
		Work->resize(900, 600);
		Work->layout()->setMenuBar(Work->getMenuBar());
		Work->show();
	}
}



void Supervision::PushVisiter()
{
	access = new Users();
	access->setWindowTitle(RUS("Запрос пароля"));
	access->setLayout(access->getLayout());

	if (access->exec() == QDialog::Accepted)// Пользователь ввел пароль
	{
		QString Username = access->getUsernameLine()->text();
		QString Password = access->getPasswordLine()->text();
		DB->passwordVerification(Username, Password);

	}
	if (DB->getTruePassword() == true)
	{
		Visit->setWindowTitle(RUS("База данных посетителей"));
		Visit->resize(600, 500);
		Visit->layout()->setMenuBar(Visit->getMenuBar());
		Visit->show();
	}
}

void Supervision::slotAbout()
{
    QMessageBox::about(this, RUS("О приложении"),
	RUS("<h2> SeekFace 1.1</h2>"
	"<p> Copyright &copy; 2018 БГУИР.</p>"
	"<p> SeekFace - система распознавания лиц.<br>"
	" Разработчик: Роденя Е.И.(ФИНО 300541)  </p>"));
}

void Supervision::slotHelp()
{
	helpBrowser = new InfoBrowser("doc", "./Resources/site/index1.html");
	helpBrowser->resize(600, 400);
	helpBrowser->show();
}


