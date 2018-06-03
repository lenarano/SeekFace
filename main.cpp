#include <QApplication>



#include "Supervision.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Supervision Seek;
	Seek.show();
	
	
	return app.exec();
}
