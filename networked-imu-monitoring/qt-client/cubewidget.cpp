#include "cubewidget.h"

#include <QPainter>
#include <QVector3D>
#include <array>

CubeWidget::CubeWidget(QWidget *parent)
    : QOpenGLWidget(parent),
    m_quaternion(1.0f, 0.0f, 0.0f, 0.0f)
{
}

void CubeWidget::setQuaternion(float q0, float q1, float q2, float q3)
{
    m_quaternion = QQuaternion(q0, q1, -q3, q2).normalized();
    update();
}

void CubeWidget::initializeGL()
{
    // Nothing special needed for now
}
void CubeWidget::paintGL()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(20, 20, 25));

    const std::array<QVector3D, 8> vertices = {
        QVector3D(-1, -1, -1),
        QVector3D( 1, -1, -1),
        QVector3D( 1,  1, -1),
        QVector3D(-1,  1, -1),
        QVector3D(-1, -1,  1),
        QVector3D( 1, -1,  1),
        QVector3D( 1,  1,  1),
        QVector3D(-1,  1,  1)
    };

    const std::array<std::pair<int, int>, 12> edges = {{
        {0,1}, {1,2}, {2,3}, {3,0},
        {4,5}, {5,6}, {6,7}, {7,4},
        {0,4}, {1,5}, {2,6}, {3,7}
    }};

    std::array<QPointF, 8> projectedPoints;

    const float scale = 180.0f;
    const float cameraDistance = 5.0f;
    const float centerX = width() / 2.0f;
    const float centerY = height() / 2.0f;

    for (int i = 0; i < 8; ++i) {
        QVector3D rotated = m_quaternion.rotatedVector(vertices[i]);

        float z = rotated.z() + cameraDistance;
        if (z < 0.1f) {
            z = 0.1f;
        }

        float x2d = centerX + (rotated.x() * scale / z);
        float y2d = centerY - (rotated.y() * scale / z);

        projectedPoints[i] = QPointF(x2d, y2d);
    }

    painter.setPen(QPen(Qt::white, 2));

    for (const auto &edge : edges) {
        painter.drawLine(projectedPoints[edge.first], projectedPoints[edge.second]);
    }

    painter.setPen(QPen(Qt::green, 2));
    painter.drawText(10, 20, "3D IMU Orientation");
}