/*
* Copyright (C) 2016
* Computer Graphics Group, The Institute of Computer Graphics and Algorithms, TU Wien
* Written by Tobias Klein <tklein@cg.tuwien.ac.at>
* All rights reserved.
*/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "Camera.h"
#include "PdbLoader.h"
#include "BufferTypes.h"
#include "OpenGLEngine/Graphics.h"
#include "MoleculesScene.h"

#include <QOpenGLDebugLogger>
#include <QGLShader>
#include <QFileSystemWatcher>
#include <QElapsedTimer>
#include <QTimer>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <memory>

class MainWindow;

class GLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	enum RenderMode
	{
		NONE,
		PDB,
		NETCDF
	};

public:
	GLWidget(QWidget* parent, MainWindow* mainWindow);
	virtual ~GLWidget();

	void MoleculeRenderMode(std::vector<std::vector<Atom>>* animation);

	void PlayAnimation();
	void PauseAnimation();
	void SetAnimationFrame(int frameNumber);

	QImage GetImage();
	void SetAmbientFactor(float value);
	void SetDiffuseFactor(float value);
	void SetSpecularFactor(float value);
	void SetIsImposerRendering(bool value);

public slots:
	void Cleanup();

protected:

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

protected slots:

	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;
	void fileChanged(const QString& path);

private:
	void InitializeFileWatcher();
	static void InitializeGLSW();
	void DrawMolecules();
	void AllocateGPUBuffer(int frameNumber);
	void CalculateFPS();
	
	void SetupDebugger();
	static void OnMessageLogged(QOpenGLDebugMessage message);

private:
		
	// CPU
	std::vector<std::vector<Atom> > *m_animation;
	std::vector<glm::vec3> m_positions;
	std::vector<float> m_radii;
	std::vector<glm::vec3> m_colors;

	// ------------------------------
	
	QPoint m_lastPos;
	
	QFileSystemWatcher* m_fileWatcher;

	int m_currentFrame = 0;
	bool m_isPlaying;
	qint64 m_lastTime;

	QElapsedTimer m_animationTimer;

	// vars to measure fps
	size_t m_frameCount;
	size_t m_fps;
	qint64 m_previousTimeFPS;
	QElapsedTimer m_fpsTimer;
	
	// Triggers the rendering events
	QTimer m_paintTimer;

	MainWindow* m_mainWindow;

private:
	Camera m_camera;
	size_t m_moleculesCount = 0;
	RenderMode m_renderMode = RenderMode::NONE;
	float m_ambientFactor = 0.05f;
	float m_diffuseFactor = 0.5f;
	float m_specularFactor = 0.3f;
	bool m_isImposerRendering = true;

	OpenGLEngine::Graphics m_graphics;
	RTVis::MoleculesScene m_moleculesScene;

	std::unique_ptr<QOpenGLDebugLogger> m_logger;
};

#endif