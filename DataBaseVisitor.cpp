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
	groupBox = new QGroupBox(RUS("База данных посетителей"), this);

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

	//Инициализируем модель для представления данных с заданием названий колонок
	setupModel(TABLE_VISITOR,
		QStringList() << RUS("id")
		<< RUS("Изображение")
		<< RUS("Распознавание")
		<< RUS("Дата")
		<< RUS("Время")
	);

	mainWidget = new QWidget();
	setCentralWidget(mainWidget);
	mainWidget->setLayout(layoutLabel);
}


void DataBaseVisitor::createActions()
{
	deleteAct = new QAction(RUS("У&далить..."), this);
	deleteAct->setIcon(QPixmap("./Resources/image/delete.png"));
	deleteAct->setStatusTip(RUS("Удалить выделенных посетителей из базы данных"));
	deleteAct->setShortcut(tr("Ctrl+D"));
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteVisitor()));

	exitAct = new QAction(RUS("В&ыход"), this);
	exitAct->setIcon(QPixmap("./Resources/image/exit.png"));
	exitAct->setStatusTip(RUS("Завершение работы с БД"));
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

}

void DataBaseVisitor::createMenus()
{
	visitorMenu = new QMenu(RUS("Посетители"));
	visitorMenu->addAction(deleteAct);

	exitMenu = new QMenu(RUS("Выход"));
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


//Метод для инициализации модели представления данных
void DataBaseVisitor::setupModel(const QString &tableName, const QStringList &headers)
{
	//Производим инициализацию модели с установкой имени таблицы в БД, по которому будет производится обращение в таблице
	modelVisitor = new TableModel(this);
	modelVisitor->setTable(tableName);
	//Устанавливаем названия колонок в таблице с сортировкой данных
	for (int i = 0, j = 0; i < modelVisitor->columnCount(); i++, j++)
	{
		modelVisitor->setHeaderData(i, Qt::Horizontal, headers[j]);
	}
}

void  DataBaseVisitor::createUI()
{
	tableView->setModel(modelVisitor);     // Устанавливаем модель на TableView
	tableView->verticalHeader()->setDefaultSectionSize(120);
	tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);// Разрешаем выделение строк
	tableView->setSelectionMode(QAbstractItemView::SingleSelection);// Устанавливаем режим выделения лишь одной строки в таблице
	tableView->horizontalHeader()->resizeSection(1, 120);
	tableView->horizontalHeader()->resizeSection(2, 160);
	tableView->horizontalHeader()->resizeSection(3, 120);
	tableView->horizontalHeader()->resizeSection(4, 120);
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Запрещаем редактирование
	tableView->horizontalHeader()->setStretchLastSection(true);     // Растягиваем последнюю колонку по всему tableView
	
	modelVisitor->select(); // Делаем выборку данных из таблицы
}

void DataBaseVisitor::deleteVisitor()
{
	QModelIndex current = tableView->currentIndex();
	int id = current.sibling(current.row(), 0/*Столбец с ключем*/).data().toInt();
	QSqlQuery query(QString("delete from VisitorTable where id = %1").arg(id));
	modelVisitor->select();
}

