#include "DataBaseWorker.h"



DataBaseWorker::DataBaseWorker(DataBase  *s) :db(s), idRezulFace(0)
{
	FaceDesk = new FindFace("DataBase");
	createActions();
	createMenus();
	createForm();
	createUI();

}

DataBaseWorker::~DataBaseWorker()
{

}

DataBase  * DataBaseWorker::getDb() { return db; }

QMenuBar * DataBaseWorker::getMenuBar() { return menuBar; }

int DataBaseWorker::getIdRezulFace() { return idRezulFace;}

QString DataBaseWorker::getIDname() { return IDname; }

QString DataBaseWorker::getIDdepartment() { return IDdepartment; }

QString DataBaseWorker::getIDposition() { return IDposition; }

QString DataBaseWorker::getIDEmail() { return IDEmail; }

QString DataBaseWorker::getIDPhone() { return IDPhone; }

void DataBaseWorker::createForm()
{
	QGroupBox *groupBox = new QGroupBox(RUS("���� ������ �����������"), this);

	LabelImg = new QLabel;
	LabelImg->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	LabelImg->setScaledContents(true);

	QScrollArea *scrollImg = new QScrollArea;
	scrollImg->setBackgroundRole(QPalette::Dark);
	scrollImg->setWidget(LabelImg);
	scrollImg->setWidgetResizable(true);

	tableView = new QTableView;
	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(tableView);
	layout->addWidget(scrollImg);

	groupBox->setLayout(layout);

	QPushButton* pushButton = new QPushButton(RUS(""));
	QVBoxLayout* layoutLabel = new QVBoxLayout;
	layoutLabel->addWidget(groupBox);
	layoutLabel->addWidget(pushButton);

	//�������������� ������ ��� ������������� ������ � �������� �������� �������
	setupModel(TABLE_WORKER,
		QStringList() << RUS("id")
		<< RUS("�����������")
		<< RUS("����������")
		<< RUS("������� ���")
		<< RUS("�����")
		<< RUS("���������")
		<< RUS("�������")
		<< RUS("E-mail")
		<< RUS("����������")
	);

	mainWidget = new QWidget();
	setCentralWidget(mainWidget);
	mainWidget->setLayout(layoutLabel);
}

void DataBaseWorker::createActions()
{
	openAct = new QAction(RUS("�&��������..."), this);
	openAct->setIcon(QPixmap("./Resources/image/add.jpg"));
	openAct->setStatusTip(RUS("��������� ���������� � ����� ���������� "));
	openAct->setShortcut(tr("Ctrl+O"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(saveWorker()));

	deleteAct = new QAction(RUS("�&������..."), this);
	deleteAct->setIcon(QPixmap("./Resources/image/delete.png"));
	deleteAct->setStatusTip(RUS("������� ����������� ���������� �� ���� ������"));
	deleteAct->setShortcut(tr("Ctrl+D"));
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteWorker()));

	exitAct = new QAction(RUS("�&����"), this);
	exitAct->setIcon(QPixmap("./Resources/image/exit.png"));
	exitAct->setStatusTip(RUS("���������� ������ � ��"));
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
	
}

void DataBaseWorker::createMenus()
{
	workerMenu = new QMenu(RUS("����������"));
	workerMenu->addAction(openAct);
	workerMenu->addAction(deleteAct);

	exitMenu = new QMenu(RUS("�����"));
	exitMenu->addAction(exitAct);
	menuBar = new QMenuBar(this);

	menuBar->addMenu(workerMenu);
	menuBar->addMenu(exitMenu);

	labelMenu = new QLabel(statusBar());
	labelFile = new QLabel();

	statusBar()->setSizeGripEnabled(false);
	statusBar()->addWidget(labelMenu, 1);
	statusBar()->addWidget(labelFile, 2);


}

//����� ��� ������������� ������ ������������� ������
void DataBaseWorker::setupModel(const QString &tableName, const QStringList &headers)
{
	//���������� ������������� ������ � ���������� ����� ������� � ��, �� �������� ����� ������������ ��������� � �������
	modelWorker = new QSqlTableModel(this);
	modelWorker->setTable(tableName);
	//������������� �������� ������� � ������� � ����������� ������
	for (int i = 0, j = 0; i < modelWorker->columnCount(); i++, j++)
	{
		modelWorker->setHeaderData(i, Qt::Horizontal, headers[j]);
	}
}

void  DataBaseWorker::createUI()
{
	tableView->setModel(modelWorker);     // ������������� ������ �� TableView
	tableView->setColumnHidden(0, true);    // �������� ������� � id �������
	tableView->setColumnHidden(1, true);    // �������� ������� � ������������
	tableView->setColumnHidden(2, true);    // �������� ������� � ������������
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);// ��������� ��������� �����
	tableView->setSelectionMode(QAbstractItemView::SingleSelection);// ������������� ����� ��������� ���� ����� ������ � �������
	tableView->resizeColumnsToContents();
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // ��������� ��������������
	tableView->horizontalHeader()->setStretchLastSection(true);     // ����������� ��������� ������� �� ����� tableView
																	//���������� ������ �� ��������� ������ ������� ������ � �������� ����� ��� ��������� ����������� � QLabel
	connect(tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this, SLOT(slotCurrentPic(QModelIndex)));
	modelWorker->select(); // ������ ������� ������ �� �������
}

void DataBaseWorker::saveWorker()
{
	Card worker(this);
	worker.setWindowTitle(RUS("����� ���������"));
	worker.resize(440, 350);
	worker.layout()->setMenuBar(worker.getMyBar());
	switch (worker.exec())
	{
	case QDialog::Accepted:
	{
		QString name = worker.getInput(worker.getNameLine());
		QString department = worker.getInput(worker.getDepartmentLine());
		QString position = worker.getInput(worker.getPositionLine());
		QString Email = worker.getInput(worker.getEmailLine());
		QString Phone = worker.getInput(worker.getPhoneLine());
		if (name.isEmpty() || department.isEmpty() || position.isEmpty() || Email.isEmpty() || Phone.isEmpty() || (!worker.getIfIDFoto()))
		{
			QMessageBox::information(this, RUS("��������:�������� �� ���������"), RUS("���������� � �� ��������� ��������!"));
			break;
		}
		//������ � �� ����������� QByteArray
		matFoto = qimage2mat(worker.getImg());
		std::vector<dlib::matrix<dlib::rgb_pixel>> faces = FaceDesk->detectorFaceMaxOpim(matFoto);
		dlib::matrix<float, 0, 1> face_desc = FaceDesk->descriptorFace1(faces);
		QVector<float> face128 = convertDlimToStdVector(face_desc);
		QByteArray deckriptor;
		deckriptor.clear();
		deckriptor.append(QByteArray::fromRawData(reinterpret_cast<const char*>(face128.data()), face128.size() * sizeof(float)));
		//������ � �� ����
		QPixmap::fromImage(worker.getImg());
		QByteArray inByteArray;// ������ ������ QByteArray ��� ���������� �����������;                             
		QBuffer inBuffer(&inByteArray);// ���������� ����������� ���������� ����� ������
		inBuffer.open(QIODevice::WriteOnly);// ��������� ������
		worker.getImg().save(&inBuffer, "PNG");// ���������� inPixmap � inByteArray
		db->insertIntoTable(inByteArray, deckriptor, name, department, position, Phone, Email, QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss"));// ���������� �������� � ���� ������
		modelWorker->select();// ������ ������� ������� �� ���� ������
		break;
	}
	case QDialog::Rejected:
	{
		break;
	}
	}
}

double DataBaseWorker::faceSearchDB(QVector<float> my128Face)
{
	double summMin = 1;
	//printVector(my128Face);
	dataFases = db->selectFromTable();
	for (QMap<int, QVector<float>>::iterator it = dataFases.begin(); it != dataFases.end(); it += 1)
	{
		double sumSq = 0;
		//printVector(it.value());
		QVector<float>::iterator iterVector = it.value().begin();//��������� �� ������
		for (int i = 0; i < 128; i++) //������ ��������� ����������
		{
			sumSq += pow((my128Face[i] - iterVector[i]), 2);
		}
		double summ = sqrt(sumSq);
		if (summMin > summ)//���� ����������� ��������
		{
			summMin = summ;
			idRezulFace = it.key();//��������� ����� ����� ��� ���������� ������� � ��
		}
		//��� ���� � itRezul �������� ����� ����� ��� ������� � ��
		//std::cout << summ << " " << summMin << " " << idRezulFace << " " << std::endl;
	}
	std::cout << summMin << " " << idRezulFace << " " << std::endl;
	return summMin;
}

void DataBaseWorker::printVector(QVector<float> my128Face)
{
	int count = 0;
	for (QVector<float>::iterator it = my128Face.begin(); it != my128Face.end(); it += 1)
	{
		count++;
		std::cout << std::setprecision(3) << std::setw(8) << *it << " ";
		if (count == 8) { std::cout << std::endl; count = 0; }
	}
	std::cout << std::endl;
}


void DataBaseWorker::selectIDInDB()
{
	db->selectFromTableIDFace(idRezulFace);
	IDname = db->getSurname();
	IDdepartment = db->getDepartment();
	IDposition = db->getPosition();
	IDEmail = db->getPhone();
	IDPhone = db->getEmail();
}


void DataBaseWorker::deleteWorker()
{
	int count = tableView->selectionModel()->selectedRows().count();
	std::cout << count << " ";
	for (int i = 0; i < count; i++)
		tableView->model()->removeRow(tableView->selectionModel()->selectedRows().at(i).row(), QModelIndex());
	modelWorker->select();// ������ ������� ������� �� ���� ������
}

QVector<float> DataBaseWorker::convertDlimToStdVector(dlib::matrix<float, 0, 1> face128)
{
	QVector<float> x;
	for (unsigned int r = 0; r < face128.nr(); r += 1)
	{
		x.push_back(face128(r, 0));
	}
	int count = 0;
	for (QVector<float>::iterator it = x.begin(); it != x.end(); it += 1)
	{
		count++;
		std::cout << std::setprecision(3) << std::setw(8) << *it << " ";
		if (count == 8) { std::cout << std::endl; count = 0; }
	}
	std::cout << std::endl;

	return x;
}

cv::Mat DataBaseWorker::qimage2mat(const QImage& qimage)
{
	cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
	cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3);
	int from_to[] = { 0,0,  1,1,  2,2 };
	cv::mixChannels(&mat, 1, &mat2, 1, from_to, 3);
	return mat2;
}

void DataBaseWorker::slotCurrentPic(QModelIndex index)
{
	QPixmap outPixmap = QPixmap();
	// ������ QPixmap, �������� ������ �� ����������� �� ������� � �������� QByteArray � �������� �� � QPixmap
	outPixmap.loadFromData(modelWorker->data(modelWorker->index(index.row(), 1)).toByteArray());
	LabelImg->setPixmap(outPixmap.scaled(400, 300));// ������������� ����������� � QLabel
}



