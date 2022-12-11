#include <QApplication>
#include "projectwindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/icons/mainicon"));

	ProjectWindow projectWindow;
	projectWindow.setWindowTitle("Unity Project Archiver");

	projectWindow.show();
	return app.exec();
}
