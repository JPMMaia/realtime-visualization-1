/*
* Copyright (C) 2016
* Computer Graphics Group, The Institute of Computer Graphics and Algorithms, TU Wien
* Written by Tobias Klein <tklein@cg.tuwien.ac.at>
* All rights reserved.
*/

#include "MainWindow.h"
#include "streamserver.h"

#include <QApplication>


int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	application.setApplicationName("RTVis");

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	QSurfaceFormat::setDefaultFormat(format);

	MainWindow mainWindow;
	mainWindow.show();

	auto server = new StreamServer(1234, false, mainWindow, application.devicePixelRatio());
	QObject::connect(server, &StreamServer::closed, &application, &QCoreApplication::quit);

	return application.exec();
}
