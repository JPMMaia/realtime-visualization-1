/*
* Copyright (C) 2016
* Computer Graphics Group, The Institute of Computer Graphics and Algorithms, TU Wien
* Written by Tobias Klein <tklein@cg.tuwien.ac.at>
* All rights reserved.
*/

#include "MainWindow.h"

#include <QFileDialog>
#include <qmessagebox.h>
#include <QPainter>
#include <QXmlStreamReader>
#include <QDomDocument>

#include "PdbLoader.h"
#include "NetCDFLoader.h"




MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_Ui = new Ui_MainWindow();
	m_Ui->setupUi(this);
	QLayout *layout = m_Ui->controls->layout();
	layout->setAlignment(Qt::AlignTop);
	m_Ui->controls->setLayout(layout);
		
	m_glWidget = new GLWidget(this, this);
	m_Ui->glLayout->addWidget(m_glWidget);
	

	connect(m_Ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileAction()));
	connect(m_Ui->actionClose, SIGNAL(triggered()), this, SLOT(closeAction()));

	connect(m_Ui->frame_slider, SIGNAL(valueChanged(int)), this, SLOT(frameChanged(int)));

	// shading
	connect(m_Ui->ambientSpinBox, SIGNAL(valueChanged(double)), this, SLOT(ambientChanged(double)));
	connect(m_Ui->diffuseSpinBox, SIGNAL(valueChanged(double)), this, SLOT(diffuseChanged(double)));
	connect(m_Ui->specularpinBox, SIGNAL(valueChanged(double)), this, SLOT(specularChanged(double)));

	// render mode
	connect(m_Ui->imposter_switch, SIGNAL(currentIndexChanged(int)), this, SLOT(renderModeChanged(int)));

	// animation
	connect(m_Ui->playButton, SIGNAL(clicked()), this, SLOT(playAnimation()));
	connect(m_Ui->pauseButton, SIGNAL(clicked()), this, SLOT(pauseAnimation()));


	m_Ui->memSizeLCD->setPalette(Qt::darkGreen);
	m_Ui->usedMemLCD->setPalette(Qt::darkGreen);
	m_Ui->fpsLCD->setPalette(Qt::darkGreen);

}

MainWindow::~MainWindow()
{
}


//-------------------------------------------------------------------------------------------------
// Slots
//-------------------------------------------------------------------------------------------------

size_t MainWindow::getFrameCount() const
{
	return m_animation.size();
}

void MainWindow::openFileAction()
{
	QString filename = QFileDialog::getOpenFileName(this, "Data File", 0, tr("Data Files (*.nc)"));

	if (!filename.isEmpty())
	{
		// store filename
		m_FileType.filename = filename;
		std::string fn = filename.toStdString();
		bool success = false;

		// progress bar and top label
		m_Ui->progressBar->setEnabled(true);
		m_Ui->labelTop->setText("Loading data ...");

		if (fn.substr(fn.find_last_of(".") + 1) == "nc")		// LOAD NetCDF DATA
		{
			
			int nrFrames;
			success = NetCDFLoader::readData(filename, m_animation, &nrFrames, m_Ui->progressBar);
					
			m_Ui->frame_slider->setMaximum(nrFrames);
			m_glWidget->MoleculeRenderMode(&m_animation);

		}

		m_Ui->progressBar->setEnabled(false);

		// status message
		if (success)
		{
			QString type;
			if (m_FileType.type == NETCDF) type = "NETCDF";
			m_Ui->labelTop->setText("File LOADED [" + filename + "] - Type [" + type + "]");
		}
		else
		{
			m_Ui->labelTop->setText("ERROR loading file " + filename + "!");
			m_Ui->progressBar->setValue(0);
		}
	}
}

void MainWindow::frameChanged(int value)
{
	if (value < m_animation.size()) {
		m_glWidget->SetAnimationFrame(value);
	}
	m_glWidget->update();
}

void MainWindow::ambientChanged(double value)
{
	m_glWidget->SetAmbientFactor(static_cast<float>(value));
	m_glWidget->update();
}
void MainWindow::diffuseChanged(double value)
{
	m_glWidget->SetDiffuseFactor(static_cast<float>(value));
	m_glWidget->update();
}
void MainWindow::specularChanged(double value)
{
	m_glWidget->SetSpecularFactor(static_cast<float>(value));
	m_glWidget->update();
}

void MainWindow::renderModeChanged(int index)
{
	if (index == 0) {
		m_glWidget->SetIsImposerRendering(true);
	}
	else {
		m_glWidget->SetIsImposerRendering(false);
	}
	m_glWidget->update();
}


void MainWindow::closeAction()
{
	close();
}

void MainWindow::playAnimation()
{
	m_glWidget->PlayAnimation();
}

void MainWindow::pauseAnimation()
{
	m_glWidget->PauseAnimation();
}

void MainWindow::setAnimationFrameGUI(int frame)
{
	m_Ui->frame_slider->setValue(frame);
}

void MainWindow::displayTotalGPUMemory(float size)
{
	m_Ui->memSizeLCD->display(size);
}
void MainWindow::displayUsedGPUMemory(float size)
{
	m_Ui->usedMemLCD->display(size);
}

void MainWindow::displayFPS(int fps)
{
	m_Ui->fpsLCD->display(fps);
}
