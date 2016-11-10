/*
* Copyright (C) 2016
* Computer Graphics Group, The Institute of Computer Graphics and Algorithms, TU Wien
* Written by Tobias Klein <tklein@cg.tuwien.ac.at>
* All rights reserved.
*/

#include "GLWidget.h"

#include <array>
#include <qopenglwidget.h>
#include <QMouseEvent>
#include <QDir>
#include <gl/GLU.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glsw.h"
#include "MainWindow.h"
#include "EngineException.h"

#include <QCoreApplication>
#include "MoleculesRenderItem.h"

const float msPerFrame = 50.0f;

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

GLWidget::GLWidget(QWidget* parent, MainWindow* mainWindow) :
	QOpenGLWidget(parent),
	m_mainWindow(mainWindow),
	m_graphics(&m_camera)
{
	InitializeFileWatcher();
	InitializeGLSW();
}
GLWidget::~GLWidget()
{
	glswShutdown();
}

void GLWidget::MoleculeRenderMode(std::vector<std::vector<Atom>>* animation)
{
	// Makes the widget's rendering context the current OpenGL rendering context:
	QOpenGLWidget::makeCurrent();

	m_animation = animation;
	m_renderMode = RenderMode::NETCDF;

	// Allocate GPU memory for frame 0:
	AllocateGPUBuffer(0);
}

void GLWidget::PlayAnimation()
{
	m_animationTimer.start();
	m_lastTime = 0;
	m_isPlaying = true;
}
void GLWidget::PauseAnimation()
{
	m_isPlaying = false;
}
void GLWidget::SetAnimationFrame(int frameNumber)
{
	m_currentFrame = frameNumber;
	AllocateGPUBuffer(frameNumber);
}

QImage GLWidget::GetImage()
{
	return QOpenGLWidget::grabFramebuffer();
}

void GLWidget::SetAmbientFactor(float value)
{
	m_ambientFactor = value;
	m_graphics.SetLightAmbientIntensity(value);
}
void GLWidget::SetDiffuseFactor(float value)
{
	m_diffuseFactor = value;
	m_graphics.SetLightDiffuseIntensity(value);
}
void GLWidget::SetSpecularFactor(float value)
{
	m_specularFactor = value;
}
void GLWidget::SetIsImposerRendering(bool value)
{
	m_isImposerRendering = value;
}

void GLWidget::Cleanup()
{
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
	m_lastPos = event->pos();
}
void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
	int dx = event->x() - m_lastPos.x();
	int dy = event->y() - m_lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		m_camera.rotateAzimuth(dx / 100.0f);
		m_camera.rotatePolar(dy / 100.0f);
	}

	if (event->buttons() & Qt::RightButton) {
		m_camera.rotateAzimuth(dx / 100.0f);
		m_camera.rotatePolar(dy / 100.0f);
	}
	m_lastPos = event->pos();
	update();
}
void GLWidget::wheelEvent(QWheelEvent* event)
{
	m_camera.zoom(event->delta() / 30);
	update();
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Space:
	{
		break;
	}
	default:
	{
		event->ignore();
		break;
	}
	}
}
void GLWidget::keyReleaseEvent(QKeyEvent*)
{
}

void GLWidget::initializeGL()
{
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::Cleanup);

	QWidget::setFocusPolicy(Qt::FocusPolicy::ClickFocus);

	// Initialize graphics component:
	m_graphics.Initialize();

	// Display GPU memory data:
	{
		auto total_mem_kb = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
		&total_mem_kb);

		auto cur_avail_mem_kb = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
		&cur_avail_mem_kb);

		auto total_mem_mb = float(total_mem_kb) / 1024.0f;

		m_mainWindow->displayTotalGPUMemory(total_mem_mb);
		m_mainWindow->displayUsedGPUMemory(0);
	}

	// Setup timers:
	{
		connect(&m_paintTimer, SIGNAL(timeout()), this, SLOT(update()));
		m_paintTimer.start(16);
		m_fpsTimer.start();
	}
}
void GLWidget::resizeGL(int width, int height)
{
	m_graphics.OnResize(width, height);

	m_camera.setAspect(static_cast<float>(width) / height);
}
void GLWidget::paintGL()
{
	CalculateFPS();

	switch (m_renderMode)
	{
	case(RenderMode::NONE):
		break;

	case(RenderMode::PDB):
		// TODO:
		break;

	case(RenderMode::NETCDF):
		DrawMolecules();
		break;

	default:
		break;
	}
}
void GLWidget::fileChanged(const QString& path)
{
	// Reboot glsw, otherwise it will use the old cached shader:
	glswShutdown();
	InitializeGLSW();
	
	QWidget::update();
}

void GLWidget::InitializeFileWatcher()
{
	m_fileWatcher = new QFileSystemWatcher(this);
	connect(m_fileWatcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));

	// Watch all shader of the shader folder.
	// Every time a shader changes it will be recompiled on the fly.
	QDir shaderDir(QCoreApplication::applicationDirPath() + "/../../src/shader/");
	auto files = shaderDir.entryInfoList();
	qDebug() << "List of shaders:";
	foreach(QFileInfo file, files)
	{
		if (file.isFile())
		{
			qDebug() << file.fileName();
			m_fileWatcher->addPath(file.absoluteFilePath());
		}
	}
}
void GLWidget::InitializeGLSW()
{
	glswInit();
	auto str = QCoreApplication::applicationDirPath() + "/../../src/shader/";
	auto ba = str.toLatin1();
	const char *shader_path = ba.data();
	glswSetPath(shader_path, ".glsl");
	glswAddDirectiveToken("", "#version 330");
}
void GLWidget::DrawMolecules()
{
	// Animate frames:
	if (m_isPlaying)
	{
		auto elapsed = m_animationTimer.elapsed() - m_lastTime;

		elapsed -= msPerFrame;
		while (elapsed > 0)
		{
			m_currentFrame++;
			m_lastTime = m_animationTimer.elapsed();
			elapsed -= msPerFrame;

		}
		if (m_currentFrame >= (*m_animation).size())
		{
			m_currentFrame = static_cast<int>(m_animation->size() - 1);
			m_isPlaying = false;
		}

		m_mainWindow->setAnimationFrameGUI(m_currentFrame);
		AllocateGPUBuffer(m_currentFrame);
	}

	if (m_isImposerRendering)
	{
		m_graphics.Update();
		m_graphics.Render();
	}
	else
	{
		// Clear back and depth buffers:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//simplistic implementation. 
		auto m_mrAtoms = (*m_animation)[m_currentFrame].size();

		m_ambientFactor = 0.05f;
		m_diffuseFactor = 0.5f;
		m_specularFactor = 0.3f;


		GLfloat light_ambient[] = { m_ambientFactor,m_ambientFactor, m_ambientFactor, 1.0f };
		GLfloat light_diffuse[] = { m_diffuseFactor, m_diffuseFactor, m_diffuseFactor, 1.0f };
		GLfloat light_specular[] = { m_specularFactor, m_specularFactor, m_specularFactor, 1.0f };

		GLfloat light_position[] = { 0.0f, 0.0f, -100.0f, 1.0f };


		//the folliwng is not that important.

		glLoadIdentity();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

		glDepthFunc(GL_LEQUAL);
		glClearDepth(1.0f);

		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

		glEnable(GL_LIGHT0);

		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);



		//transformations etc.


		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(m_camera.getProjectionMatrix()));
		glMatrixMode(GL_MODELVIEW);
		glMultMatrixf(glm::value_ptr(m_camera.getViewMatrix()));

		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		for (size_t i = 0; i < m_mrAtoms; i++)
		{
			auto atom = (*m_animation)[m_currentFrame][i];
			glPushMatrix();
			GLUquadric *quad;
			quad = gluNewQuadric();
			//set color and position
			glColor4f(atom.color.r, atom.color.g, atom.color.b, 1);
			glTranslatef(atom.position.x, atom.position.y, atom.position.z);
			gluSphere(quad, atom.radius, 40, 40);
			glPopMatrix();
			gluDeleteQuadric(quad);
		}
	}
}
void GLWidget::AllocateGPUBuffer(int frameNumber)
{
	// Makes the widget's rendering context the current OpenGL rendering context
	makeCurrent();

	m_moleculesCount = (*m_animation)[frameNumber].size();

	// Load atoms:
	{
		m_positions.clear();
		m_radii.clear();
		m_colors.clear();

		for (size_t i = 0; i < m_moleculesCount; ++i)
		{
			auto atom = (*m_animation)[frameNumber][i];
			m_positions.push_back(atom.position);
			m_radii.push_back(atom.radius);
			m_colors.push_back(atom.color);
		}
	}

	// Allocating GPU memory for rendering:
	m_moleculesScene.Initialize(&m_graphics, (*m_animation)[frameNumber]);

	// Display used GPU memory:
	{
		// Get total memory:
		auto totalMemoryKB = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &totalMemoryKB);
		auto totalMemoryMB = float(totalMemoryKB) / 1024.0f;

		// Get current available memory:
		auto currrentAvailableMemoryKB = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &currrentAvailableMemoryKB);
		auto currentAvailableMemoryMB = float(currrentAvailableMemoryKB) / 1024.0f;

		// Display used GPU memory:
		m_mainWindow->displayUsedGPUMemory(totalMemoryMB - currentAvailableMemoryMB);
	}
}
void GLWidget::CalculateFPS()
{
	m_frameCount++;

	auto currentTime = m_fpsTimer.elapsed();

	//  Calculate time passed
	auto timeInterval = currentTime - m_previousTimeFPS;

	if (timeInterval > static_cast<qint64>(1000))
	{
		// calculate the number of frames per second
		m_fps = m_frameCount / (timeInterval / 1000.0f);

		// set time
		m_previousTimeFPS = currentTime;

		// reset frame count
		m_frameCount = 0;
	}

	m_mainWindow->displayFPS(static_cast<int>(m_fps));
}

void GLWidget::SetupDebugger()
{
	QOpenGLContext* ctx = QOpenGLContext::currentContext();
	if (!ctx->hasExtension(QByteArrayLiteral("GL_KHR_debug"))) {}
		//ThrowEngineException(L"Debug logger is not supported.");

	m_logger = std::make_unique<QOpenGLDebugLogger>(this);;

	connect(
		m_logger.get(),
		SIGNAL(messageLogged(QOpenGLDebugMessage)),
		this,
		SLOT(onMessageLogged(QOpenGLDebugMessage)),
		Qt::DirectConnection
	);

	if (!m_logger->initialize()){}
		//ThrowEngineException(L"Couldn't initialize logger.");

	m_logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
	m_logger->enableMessages();
}
void GLWidget::OnMessageLogged(QOpenGLDebugMessage message)
{
	qDebug() << message;
}
