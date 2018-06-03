#include "DataBaseVisitor.h"


DataBaseVisitor::DataBaseVisitor(DataBase  *s) : db(s)
{
	createActions();
	createMenus();
	createForm();
	createUI();

}

DataBaseVisitor::~DataBaseVisitor()
{

}

TableModel* DataBaseVisitor::getModelVisitor() { return modelVisitor; }

QMenuBar* DataBaseVisitor::getMenuBar() { return menuBar; }

void DataBaseVisitor::createForm()
{
	groupBox = new QGroupBox(RUS("���� ������ �����������"), this);

	tableView = new QTableView;
	QScrollArea *scrollImg = new QScrollArea;
	scrollImg->setBackgroundRole(QPalette::Dark);
	scrollImg->setWidget(tableView);
	scrollImg->setWidgetResizable(true);

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(scrollImg);
	groupBox->setLayout(layout);

	layoutLabel = new QVBoxLayout;
	layoutLabel->addWidget(groupBox);

	//�������������� ������ ��� ������������� ������ � �������� �������� �������
	setupModel(TABLE_VISITOR,
		QStringList() << RUS("id")
		<< RUS("�����������")
		<< RUS("�������������")
		<< RUS("����")
		<< RUS("�����")
	);

	mainWidget = new QWidget();
	setCentralWidget(mainWidget);
	mainWidget->setLayout(layoutLabel);
}


void DataBaseVisitor::createActions()
{
	deleteAct = new QAction(RUS("�&������..."), this);
	deleteAct->setIcon(QPixmap("./Resources/image/delete.png"));
	deleteAct->setStatusTip(RUS("������� ���������� ����������� �� ���� ������"));
	deleteAct->setShortcut(tr("Ctrl+D"));
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteVisitor()));

	exitAct = new QAction(RUS("�&����"), this);
	exitAct->setIcon(QPixmap("./Resources/image/exit.png"));
	exitAct->setStatusTip(RUS("���������� ������ � ��"));
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

}

void DataBaseVisitor::createMenus()
{
	visitorMenu = new QMenu(RUS("����������"));
	visitorMenu->addAction(deleteAct);

	exitMenu = new QMenu(RUS("�����"));
	exitMenu->addAction(exitAct);
	menuBar = new QMenuBar(this);

	menuBar->addMenu(visitorMenu);
	menuBar->addMenu(exitMenu);

	labelMenu = new QLabel(statusBar());
	labelFile = new QLabel();

	statusBar()->setSizeGripEnabled(false);
	statusBar()->addWidget(labelMenu, 1);
	statusBar()->addWidget(labelFile, 2);


}


//����� ��� ������������� ������ ������������� ������
void DataBaseVisitor::setupModel(const QString &tableName, const QStringList &headers)
{
	//���������� ������������� ������ � ���������� ����� ������� � ��, �� �������� ����� ������������ ��������� � �������
	modelVisitor = new TableModel(this);
	modelVisitor->setTable(tableName);
	//������������� �������� ������� � ������� � ����������� ������
	for (int i = 0, j = 0; i < modelVisitor->columnCount(); i++, j++)
	{
		modelVisitor->setHeaderData(i, Qt::Horizontal, headers[j]);
	}
}

void  DataBaseVisitor::createUI()
{
	tableView->setModel(modelVisitor);     // ������������� ������ �� TableView
	tableView->verticalHeader()->setDefaultSectionSize(120);
	tableView->setColumnHidden(0, true);    // �������� ������� � id �������
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);// ��������� ��������� �����
	tableView->setSelectionMode(QAbstractItemView::SingleSelection);// ������������� ����� ��������� ���� ����� ������ � �������
	tableView->horizontalHeader()->resizeSection(1, 120);
	tableView->horizontalHeader()->resizeSection(2, 160);
	tableView->horizontalHeader()->resizeSection(3, 120);
	tableView->horizontalHeader()->resizeSection(4, 120);
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // ��������� ��������������
	tableView->horizontalHeader()->setStretchLastSection(true);     // ����������� ��������� ������� �� ����� tableView
	
	modelVisitor->select(); // ������ ������� ������ �� �������
}

void DataBaseVisitor::deleteVisitor()
{
	QModelIndex current = tableView->currentIndex();
	int id = current.sibling(current.row(), 0/*������� � ������*/).data().toInt();
	QSqlQuery query(QString("delete from VisitorTable where id = %1").arg(id));
	modelVisitor->select();
}

