#include <QApplication>
#include "projectwindow.h"

//#include "iconablebutton.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	/*IconableButton button;
	button.show();*/

	ProjectWindow projectWindow;
	projectWindow.show();

	return app.exec();
}
