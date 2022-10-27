#include "PhEdge.h"

#include <stdexcept>

using namespace ph;

PhEdge::PhEdge(PhVertex* vertexA, PhVertex* vertexB)
    : m_vertexA(vertexA)
    , m_vertexB(vertexB)
{
    if (m_vertexA && m_vertexB)
    {
        m_initialLength = differenceVector().length();
    }
    else
    {
        throw std::invalid_argument("vertexA and vertexB parameters can not be null");
    }
}

PhVertex *PhEdge::vertexA() const
{
    return m_vertexA;
}

PhVertex *PhEdge::vertexB() const
{
    return m_vertexB;
}

float PhEdge::initialLength() const
{
    return m_initialLength;
}

QVector2D PhEdge::differenceVector() const
{
    return m_vertexB->position() - m_vertexA->position();
}
