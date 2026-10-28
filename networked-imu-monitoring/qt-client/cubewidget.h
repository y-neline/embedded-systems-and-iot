#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QOpenGLWidget>
#include <QQuaternion>

class CubeWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit CubeWidget(QWidget *parent = nullptr);
    void setQuaternion(float q0, float q1, float q2, float q3);

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    QQuaternion m_quaternion;
};

#endif // CUBEWIDGET_H