#include "PhRenderer.h"

#include <QtQml/qqml.h>

#include <QPainter>
#include <QBrush>
#include <QtMath>

#include <limits>

using namespace ph;

namespace {

constexpr int UPDATE_RATE {33};

constexpr float TOUCH_EPSILON {20.f};

constexpr float ZOOM_RATE {0.0015f};
constexpr float ZOOM_MIN {0.1f};
constexpr float ZOOM_MAX {20.f};

inline float INVERSE(float number)
{
    return qPow(number, -1.f);
}

}

PhRenderer::PhRenderer(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::MiddleButton);
    startExecution();
}

void PhRenderer::paint(QPainter *painter)
{
    if (!m_solver || m_solver->empty())
    {
        return;
    }

    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(m_offset.toPoint());
    painter->scale(m_zoom, m_zoom);

    painter->setPen(QPen(Qt::black, 6.f, Qt::SolidLine, Qt::RoundCap));

    for (PhEdge* edge : m_solver->edges())
    {
        painter->drawLine(edge->vertexA()->position().toPointF(),
                          edge->vertexB()->position().toPointF());
    }

    for (PhVertex* vertex : m_solver->vertexes())
    {
        const QColor color = vertex == m_currentVertex ? Qt::yellow : (vertex->locked() ? Qt::red : Qt::gray);
        const float size = vertex == m_currentVertex ? 7.f : 5.f;
        painter->setPen(QPen(color, size, Qt::SolidLine, Qt::RoundCap));
        painter->drawPoint(vertex->position().toPointF());
    }
}

PhSolver *PhRenderer::solver() const
{
    return m_solver;
}

void PhRenderer::setSolver(PhSolver *solver)
{
    if (m_solver == solver)
    {
        return;
    }

    m_solver = solver;
    emit solverChanged();

    startExecution();
}

bool PhRenderer::timerActive() const
{
    return m_timerActive;
}

void PhRenderer::setTimerActive(bool timerActive)
{
    if (m_timerActive == timerActive)
    {
        return;
    }

    m_timerActive = timerActive;
    emit timerActiveChanged();

    if (m_timerActive)
    {
        m_timer.start(::UPDATE_RATE, this);
    }
    else
    {
        m_timer.stop();
    }
}

void PhRenderer::naiveUpdate()
{
    update();
}

void PhRenderer::smartUpdate()
{
    if (!m_timer.isActive())
    {
        naiveUpdate();
    }
}

void PhRenderer::timerEvent(QTimerEvent *event)
{
    if (m_solver && event->timerId() == m_timer.timerId())
    {
        m_solver->simulate();
    }

    QObject::timerEvent(event);
}

void PhRenderer::mousePressEvent(QMouseEvent *event)
{
    if (!m_solver)
    {
        return;
    }

    if (event->button() == Qt::LeftButton)
    {
        if (m_solver->vertexes().length() == 0)
        {
            return;
        }

        float minLength = std::numeric_limits<float>::max();
        const QVector2D mousePos = ::INVERSE(m_zoom) * (QVector2D(event->localPos()) - m_offset);

        for (PhVertex* vertex : m_solver->vertexes())
        {
            const float currentLength = mousePos.distanceToPoint(vertex->position());
            if (currentLength < minLength)
            {
                minLength = currentLength;

                if (minLength < ::TOUCH_EPSILON)
                {
                    m_currentVertex = vertex;
                }
            }
        }

        if (m_currentVertex)
        {
            m_wasLocked = m_currentVertex->locked();
            m_currentVertex->locked() = true;
        }

        smartUpdate();
    }
    else if (event->button() == Qt::MiddleButton)
    {
        m_pressOffset = m_offset - QVector2D(event->localPos());

        smartUpdate();
    }
}

void PhRenderer::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        if (m_currentVertex)
        {
            m_currentVertex->prevPosition() = m_currentVertex->position();
            m_currentVertex->position() = ::INVERSE(m_zoom) * (QVector2D(event->localPos()) - m_offset);

            smartUpdate();
        }
    }
    else if (event->buttons() == Qt::MiddleButton)
    {
        m_offset = QVector2D(event->localPos()) + m_pressOffset;

        smartUpdate();
    }
}

void PhRenderer::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    if (event->button() == Qt::LeftButton)
    {
        if (m_currentVertex)
        {
            m_currentVertex->locked() = m_wasLocked;
            m_currentVertex = nullptr;

            smartUpdate();
        }
    }
    else if (event->button() == Qt::MiddleButton)
    {
        m_pressOffset = QVector2D();
    }
}

void PhRenderer::wheelEvent(QWheelEvent *event)
{
    m_zoom = qBound(::ZOOM_MIN, m_zoom * static_cast<float>(qExp(event->angleDelta().y() * ::ZOOM_RATE)), ::ZOOM_MAX);

    smartUpdate();
}

void PhRenderer::startExecution()
{
    if (m_solver)
    {
        connect(m_solver, &PhSolver::ready, this, &PhRenderer::naiveUpdate);
        setTimerActive(true);
    }
}
