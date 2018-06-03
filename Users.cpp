#include "Users.h"




Users::Users(QWidget* parent/*= 0*/)
	: QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint)
{
	
	QGroupBox *groupBox = new QGroupBox(RUS("Доступ к базе данных"));
	QLabel *name = new QLabel("Username:");
	usernameLine = new QLineEdit;
	QLabel *password = new QLabel("Password:");
	passwordLine = new QLineEdit;
	passwordLine->setEchoMode(QLineEdit::Password);

	QHBoxLayout* layoutName = new QHBoxLayout;
	layoutName->addWidget(name);
	layoutName->addWidget(usernameLine);

	QHBoxLayout* layoutPassword = new QHBoxLayout;
	layoutPassword->addWidget(password);
	layoutPassword->addWidget(passwordLine);

	QVBoxLayout* layout1_2 = new QVBoxLayout;
	layout1_2->addLayout(layoutName);
	layout1_2->addLayout(layoutPassword);

	QPushButton* LoginBtn = new QPushButton("Login");
	connect(LoginBtn, SIGNAL(clicked()), SLOT(accept()));

	QPushButton* ClearBtn = new QPushButton("Clear");
	connect(ClearBtn, SIGNAL(clicked()), SLOT(clearForm()));

	QVBoxLayout* Btn = new QVBoxLayout;
	Btn->addWidget(LoginBtn);
	Btn->addWidget(ClearBtn);

	QHBoxLayout* layout1_2_3 = new QHBoxLayout;
	layout1_2_3->addLayout(layout1_2);
	layout1_2_3->addLayout(Btn);

	groupBox->setLayout(layout1_2_3);

	layout= new QHBoxLayout;
	layout->addWidget(groupBox);

}

QHBoxLayout* Users::getLayout() { return layout; }

QLineEdit * Users::getUsernameLine() { return usernameLine; }

QLineEdit * Users::getPasswordLine() { return passwordLine; }

void Users :: clearForm()
{
	usernameLine->setText(RUS(""));
	passwordLine->setText(RUS(""));
}