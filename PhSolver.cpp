#include "PhSolver.h"

using namespace ph;

namespace {

constexpr float DELTA_TIME = 0.2f;
constexpr int NUMBER_OF_ITERATIONS = 10;

}

PhSolver::PhSolver(QQuickItem *parent)
    : QQuickItem(parent)
{
    connect(this, &PhSolver::gridWidthChanged, this, &PhSolver::initGrid, Qt::QueuedConnection);
    connect(this, &PhSolver::gridHeightChanged, this, &PhSolver::initGrid, Qt::QueuedConnection);
    connect(this, &PhSolver::gridDensityChanged, this, &PhSolver::initGrid, Qt::QueuedConnection);
    connect(this, &PhSolver::gravityXChanged, this, &PhSolver::ready, Qt::QueuedConnection);
    connect(this, &PhSolver::gravityYChanged, this, &PhSolver::ready, Qt::QueuedConnection);
}

PhSolver::~PhSolver()
{
    clearGrid();
}

void PhSolver::initGrid()
{
    clearGrid();
    generateGrid(gridWidth(), gridHeight(), gridDensity());

    emit ready();
}

void PhSolver::simulate() // "verlet" simulation
{
    if (empty())
    {
        return;
    }

    for (PhVertex* const vertex : m_vertexes)
    {
        if (!vertex->locked())
        {
            const QVector2D cachedPosition = vertex->position();
            vertex->position() += (vertex->position() - vertex->prevPosition()) + QVector2D(gravityX(), gravityY()) * ::DELTA_TIME;
            vertex->prevPosition() = cachedPosition;
        }
    }

    for (int j = 0; j < ::NUMBER_OF_ITERATIONS; ++j)
    {
        for (PhEdge* const edge : m_edges)
        {
            const QVector2D& difference = edge->differenceVector();
            const QVector2D& direction = difference.normalized();
            const float& error = difference.length() - edge->initialLength();

            if (!edge->vertexA()->locked())
            {
                edge->vertexA()->position() += direction * (error * (edge->vertexB()->locked() ? 1.f : 0.5f));
            }

            if (!edge->vertexB()->locked())
            {
                edge->vertexB()->position() -= direction * (error * (edge->vertexA()->locked() ? 1.f : 0.5f));
            }
        }
    }

    emit ready();
}

const QVector<PhVertex*>& PhSolver::vertexes() const
{
    return m_vertexes;
}

const QVector<PhEdge*>& PhSolver::edges() const
{
    return m_edges;
}

bool PhSolver::empty() const
{
    return m_vertexes.empty() || m_edges.empty();
}

int PhSolver::gridWidth() const
{
    return m_gridWidth;
}

void PhSolver::setGridWidth(int gridWidth)
{
    if (m_gridWidth == gridWidth)
    {
        return;
    }

    m_gridWidth = gridWidth;
    emit gridWidthChanged();
}

int PhSolver::gridHeight() const
{
    return m_gridHeight;
}

void PhSolver::setGridHeight(int gridHeight)
{
    if (m_gridHeight == gridHeight)
    {
        return;
    }

    m_gridHeight = gridHeight;
    emit gridHeightChanged();
}

int PhSolver::gridDensity() const
{
    return m_gridDensity;
}

void PhSolver::setGridDensity(int gridDensity)
{
    if (m_gridDensity == gridDensity)
    {
        return;
    }

    m_gridDensity = gridDensity;
    emit gridDensityChanged();
}

float PhSolver::gravityX() const
{
    return m_gravityX;
}

void PhSolver::setGravityX(float gravityX)
{
    if (qFuzzyCompare(m_gravityX, gravityX))
    {
        return;
    }

    m_gravityX = gravityX;
    emit gravityXChanged();
}

float PhSolver::gravityY() const
{
    return m_gravityY;
}

void PhSolver::setGravityY(float gravityY)
{
    if (qFuzzyCompare(m_gravityY, gravityY))
    {
        return;
    }

    m_gravityY = gravityY;
    emit gravityYChanged();
}

void PhSolver::generateGrid(int width, int height, int density)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            const bool locked = (width == 1 || height == 1) ? (i == 0 && j == 0) : (i == 0 && (j % 2) == 0);
            m_vertexes.append(new PhVertex(j * density, i * density, locked));

            if (j > 0)
            {
                m_edges.append(new PhEdge(m_vertexes[m_vertexes.length() - 2], m_vertexes[m_vertexes.length() - 1]));
            }

            if (i > 0)
            {
                m_edges.append(new PhEdge(m_vertexes[(i - 1) * width + j], m_vertexes[i * width + j]));
            }
        }
    }
}

void PhSolver::clearGrid()
{
    qDeleteAll(m_edges);
    m_edges.clear();

    qDeleteAll(m_vertexes);
    m_vertexes.clear();
}
