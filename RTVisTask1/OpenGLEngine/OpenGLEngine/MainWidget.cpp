#include "MainWidget.h"

#include <Qt3DInput/QMouseEvent>

MainWidget::MainWidget(QWidget *parent) :
	QOpenGLWidget(parent)
{
}

void MainWidget::mousePressEvent(QMouseEvent* event)
{
	// Save mouse press position
	m_mousePressPosition = QVector2D(event->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent*)
{
	// Mouse release position - mouse press position
	//auto diff = QVector2D(event->localPos()) - m_mousePressPosition;

	// Rotation axis is perpendicular to the mouse position difference
	// vector
	//auto n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

	// Accelerate angular speed relative to the length of the mouse sweep
	//auto acc = diff.length() / 100.0;

	// Calculate new rotation axis as weighted sum
	//m_rotationAxis = (m_rotationAxis * m_angularSpeed + n * acc).normalized();

	// Increase angular speed
	//m_angularSpeed += acc;
}

void MainWidget::timerEvent(QTimerEvent*)
{
	// Update graphics component:
	m_graphics.Update();

	// Request an update:
	QWidget::update();
}

void MainWidget::initializeGL()
{
	// Initialize graphics component:
	m_graphics.Initialize();

	// Start timer:
	m_timer.start(12, this);
}

void MainWidget::resizeGL(int width, int height)
{
	m_graphics.OnResize(width, height);
}

void MainWidget::paintGL()
{
	m_graphics.Render();
}