#include <QtWidgets/QApplication>

#include "MainWidget.h"

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	QSurfaceFormat::setDefaultFormat(format);

	application.setApplicationName("cube");
	application.setApplicationVersion("0.1");

	MainWidget widget;
	widget.show();

	return application.exec();
}