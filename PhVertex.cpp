#include "PhVertex.h"

using namespace ph;

PhVertex::PhVertex(const QVector2D& position, bool locked)
    : m_position(position)
    , m_prevPosition(position)
    , m_locked(locked)
{
}

PhVertex::PhVertex(float x, float y, bool locked)
    : PhVertex(QVector2D(x, y), locked)
{
}

QVector2D& PhVertex::position()
{
    return m_position;
}

QVector2D& PhVertex::prevPosition()
{
    return m_prevPosition;
}

bool& PhVertex::locked()
{
    return m_locked;
}
