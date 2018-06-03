#include "Supervision.h"




Supervision::Supervision(QWidget* pwgt) : QMainWindow(pwgt), id(0), samePeople(false), countFace(0), countNotFace(0)
{
	DB = new DataBase();
	DB->connectToDataBase();

	Work = new DataBaseWorker(DB);
	Visit = new DataBaseVisitor(DB);
	connect(&w, SIGNAL(applied(QVector<float>)), SLOT(faceSearch(QVector<float>)));
	connect(&w, SIGNAL(ifLabelZero(QPixmap)), SLOT(LabelZero(QPixmap)));
	//���������� �����
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

	QGroupBox *camera = new QGroupBox(RUS("������"));
	camera->setLayout(Vlayout1);

	QLabel *accessDate = new QLabel(RUS("   ���� �������:"));
	accessDateLine = new QLineEdit;
	QLabel *accessTime = new QLabel(RUS("   ����� �������:"));
	accessTimeLine = new QLineEdit;
	QLabel *id = new QLabel(RUS("   ���������������:"));
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

	QGroupBox *visitorGroupBox = new QGroupBox(RUS("���������� ����"));
	visitorGroupBox->setLayout(Hlayout1);

	QLabel *name = new QLabel(RUS(" ������� ���:"));
	nameLine = new QLineEdit;
	QLabel *department = new QLabel(RUS(" �����:"));
	departmentLine = new QLineEdit;
	QLabel *position = new QLabel(RUS(" ���������:"));
	positionLine = new QLineEdit;
	QLabel *Email = new QLabel(RUS(" ����������� �����:"));
	EmailLine = new QLineEdit;
	QLabel *Phone = new QLabel(RUS(" �������:"));
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

	QGroupBox *workerGroupBox = new QGroupBox(RUS("���������� �������������"));
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
	openIDFace = new QAction(RUS("�� ����������� "), this);
	openIDFace->setIcon(QPixmap("./Resources/image/BDSotr.png"));
	openIDFace->setStatusTip(RUS("��������� ���� ������ ����������� ��� ��������� � ��������������"));
	connect(openIDFace, SIGNAL(triggered()), this, SLOT(PushWorker()));

	openViziter = new QAction(RUS("�� �����������"), this);
	openViziter->setIcon(QPixmap("./Resources/image/BDViz.png"));
	openViziter->setStatusTip(RUS("��������� ���� ������ ����������� ��� ���������"));
	connect(openViziter, SIGNAL(triggered()), this, SLOT(PushVisiter()));

	�boutAct = new QAction(RUS("���������� � ����������"), this);
	�boutAct->setIcon(QPixmap("./Resources/image/Razr.png"));
	�boutAct->setStatusTip(RUS("���������� � ����������"));
	connect(�boutAct, SIGNAL(triggered()), this, SLOT(slotAbout()));

	helpAct = new QAction(RUS("����������� ������������"), this);
	helpAct->setIcon(QPixmap("./Resources/image/help.png"));
	helpAct->setStatusTip(RUS("����������� ������ � ����������"));
	connect(helpAct, SIGNAL(triggered()), this, SLOT(slotHelp()));

	exitAct = new QAction(RUS("�&����"), this);
	exitAct->setIcon(QPixmap("./Resources/image/exit.png"));
	exitAct->setStatusTip(RUS("��������� ������ ���������"));
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

}

void Supervision::createMenus()
{
	DBFaceMenu = new QMenu(RUS("���� ������"));
	DBFaceMenu->addAction(openIDFace);
	DBFaceMenu->addAction(openViziter);
	helpMenu = new QMenu(RUS("�������"));
	helpMenu->addAction(�boutAct);
	helpMenu->addAction(helpAct);
	exitMenu = new QMenu(RUS("�����"));
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
	double evklid = Work->faceSearchDB(my128Face);//���������� � ������� ������������ � ������� id �������� ����������
	if (evklid < 0.60)
	{
		countNotFace = 0;
		std::cout << " id " << id << " Work->idRezulFace " << Work->getIdRezulFace() << std::endl;
		if (id != Work->getIdRezulFace())//���� �������� ������ �������
		{
			samePeople = true;//������ ��� �������-���������� ������ ������
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
			IDFace->setPixmap(Work->getDb()->getImageFoto());// ������������� ����������� � QLabel
			IDFace->adjustSize();
		}

		if (id == Work->getIdRezulFace() && samePeople)//����� � ���� ����������
		{
			countFace++;
			if (countFace == 2)//�����
			{
				//std::cout << " Raspoznan " << countFace << std::endl;
				Work->selectIDInDB();//������ ������� �������� �����������
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
		nameLine->setText(RUS("�� ���������"));
		departmentLine->setText(RUS("�� ���������"));
		positionLine->setText(RUS("�� ���������"));
		EmailLine->setText(RUS("�� ���������"));
		PhoneLine->setText(RUS("�� ���������"));
		idLine->setText(RUS("�� ���������"));
		IDFace->setPixmap(QPixmap("./Resources/image/Razr.png"));// ������������� ����������� � QLabel
		IDFace->adjustSize();
		if (countNotFace == 2)
		{
			//std::cout << " Ne raspoznan " << countNotFace << std::endl;
			DB->insertIntoTable(w.getPix(), RUS("�� ���������"), w.getDate(), w.getTime());
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
	IDFace->setPixmap(pix);// ������������� ����������� � QLabel
	IDFace->adjustSize();
}

void Supervision::PushWorker()
{
	access = new Users();
	access->setWindowTitle(RUS("������ ������"));
	access->setLayout(access->getLayout());
	
	if (access->exec() == QDialog::Accepted)// ������������ ���� ������
	{
		QString Username = access->getUsernameLine()->text();
		QString Password = access->getPasswordLine()->text();
		DB->passwordVerification(Username, Password);
		
	}
	if (DB->getTruePassword() == true)
	{
		Work->setWindowTitle(RUS("���� ������ �����������"));
		Work->resize(900, 600);
		Work->layout()->setMenuBar(Work->getMenuBar());
		Work->show();
	}
}



void Supervision::PushVisiter()
{
	access = new Users();
	access->setWindowTitle(RUS("������ ������"));
	access->setLayout(access->getLayout());

	if (access->exec() == QDialog::Accepted)// ������������ ���� ������
	{
		QString Username = access->getUsernameLine()->text();
		QString Password = access->getPasswordLine()->text();
		DB->passwordVerification(Username, Password);

	}
	if (DB->getTruePassword() == true)
	{
		Visit->setWindowTitle(RUS("���� ������ �����������"));
		Visit->resize(600, 500);
		Visit->layout()->setMenuBar(Visit->getMenuBar());
		Visit->show();
	}
}

void Supervision::slotAbout()
{
    QMessageBox::about(this, RUS("� ����������"),
	RUS("<h2> SeekFace 1.1</h2>"
	"<p> Copyright &copy; 2018 �����.</p>"
	"<p> SeekFace - ������� ������������� ���.<br>"
	" �����������: ������ �.�.(���� 300541)  </p>"));
}

void Supervision::slotHelp()
{
	helpBrowser = new InfoBrowser("doc", "./Resources/site/index1.html");
	helpBrowser->resize(600, 400);
	helpBrowser->show();
}


