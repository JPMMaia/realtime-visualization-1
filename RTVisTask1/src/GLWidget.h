/*
* Copyright (C) 2016
* Computer Graphics Group, The Institute of Computer Graphics and Algorithms, TU Wien
* Written by Tobias Klein <tklein@cg.tuwien.ac.at>
* All rights reserved.
*/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QGLShader>
#include <QFileSystemWatcher>
#include <QElapsedTimer>
#include <QTimer>

#include "Camera.h"
#include "PdbLoader.h"
#include "BufferTypes.h"
#include "MoleculesBuffer.h"
#include <memory>

class MainWindow;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent, MainWindow *mainWindow);
	virtual ~GLWidget();

	static void createSphere(int lats, int longs);

	void moleculeRenderMode(std::vector<std::vector<Atom> > *animation);

	void playAnimation();
	void pauseAnimation();
	void setAnimationFrame(int frameNr);

	float ambientFactor;
	float diffuseFactor;
	float specularFactor;
	bool isImposerRendering;

	inline QImage getImage()
	{
		return this->grabFramebuffer();
	}

	enum RenderMode
	{
		NONE,
		PDB,
		NETCDF
	} renderMode;

public slots:
	void cleanup();

signals:

protected:

	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

	void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;


protected slots:

	void paintGL() Q_DECL_OVERRIDE;
	void initializeGL() Q_DECL_OVERRIDE;
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;
	void fileChanged(const QString &path);

private:

	static void initglsw();

	void drawMolecules();

	void loadMoleculeShader() const;
	void allocateGPUBuffer(int frameNumber);
	void calculateFPS();

	void setupDebugger();
	static void onMessageLogged(QOpenGLDebugMessage message);

private:
	Camera m_camera;
	size_t m_moleculesCount;
		
	// CPU
	std::vector<std::vector<Atom> > *m_animation;
	std::vector<glm::vec3> m_positions;
	std::vector<float> m_radii;
	std::vector<glm::vec3> m_colors;
	std::vector<glm::vec3> m_ambOcc;
	
	// GPU
	QOpenGLShaderProgram *m_moleculesProgram;
	QOpenGLVertexArrayObject m_vao_molecules;
	QOpenGLShader *m_vertexShader;
	QOpenGLShader *m_geometryShader;
	QOpenGLShader *m_fragmentShader;

	QOpenGLBuffer m_vbo_pos;
	QOpenGLBuffer m_vbo_radii;
	QOpenGLBuffer m_vbo_colors;
	QOpenGLBuffer m_vbo_ambOcc;

	// ------------------------------
	
	QPoint m_lastPos;
	int m_projMatrixLoc;
	int m_mvMatrixLoc;
	
	QFileSystemWatcher* m_fileWatcher;

	int m_currentFrame;
	bool m_isPlaying;
	qint64 m_lastTime;

	QElapsedTimer m_AnimationTimer;

	// vars to measure fps
	size_t m_frameCount;
	size_t m_fps;
	qint64 m_previousTimeFPS;
	QElapsedTimer m_fpsTimer;
	
	// Triggers the rendering events
	QTimer m_paintTimer;

	MainWindow* m_mainWindow;

private:
	RTV::BufferTypes::PassConstants m_passConstants;
	RTV::BufferTypes::MoleculesProgramUniformLocations m_uniformLocations;
	RTV::BufferTypes::MoleculesProgramAttributeLocations m_attributeLocations;

	RTV::MoleculesBuffer m_moleculesBuffer;

	std::unique_ptr<QOpenGLDebugLogger> m_logger;
};

#endif