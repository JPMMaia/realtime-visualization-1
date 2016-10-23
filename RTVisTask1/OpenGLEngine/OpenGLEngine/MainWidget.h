#pragma once

#include "Graphics.h"

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions>
#include <QtCore/QBasicTimer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLTexture>

class MainWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit MainWidget(QWidget* parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void timerEvent(QTimerEvent* event) override;

	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

private:
	QBasicTimer m_timer;
	OpenGLEngine::Graphics m_graphics;
	
	QVector2D m_mousePressPosition;
};