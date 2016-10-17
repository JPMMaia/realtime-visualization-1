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

	QApplication app(argc, argv);
	MainWindow mainWindow;
	mainWindow.show();

	StreamServer *server = new StreamServer(1234, false, mainWindow, app.devicePixelRatio());
	QObject::connect(server, &StreamServer::closed, &app, &QCoreApplication::quit);

	return app.exec();

}
