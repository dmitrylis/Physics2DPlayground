#ifndef PHSOLVER_H
#define PHSOLVER_H

#include <QQuickItem>
#include <QVector>

#include "PhVertex.h"
#include "PhEdge.h"

namespace ph {

class PhSolver : public QQuickItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(PhSolver)

    Q_PROPERTY(int gridWidth READ gridWidth WRITE setGridWidth NOTIFY gridWidthChanged)
    Q_PROPERTY(int gridHeight READ gridHeight WRITE setGridHeight NOTIFY gridHeightChanged)
    Q_PROPERTY(int gridDensity READ gridDensity WRITE setGridDensity NOTIFY gridDensityChanged)
    Q_PROPERTY(float gravityX READ gravityX WRITE setGravityX NOTIFY gravityXChanged)
    Q_PROPERTY(float gravityY READ gravityY WRITE setGravityY NOTIFY gravityYChanged)

public:
    explicit PhSolver(QQuickItem *parent = nullptr);
    ~PhSolver();

    Q_INVOKABLE void initGrid();

    void simulate();

    const QVector<PhVertex*>& vertexes() const;
    const QVector<PhEdge*>& edges() const;

    bool empty() const;

    int gridWidth() const;
    void setGridWidth(int gridWidth);

    int gridHeight() const;
    void setGridHeight(int gridHeight);

    int gridDensity() const;
    void setGridDensity(int gridDensity);

    float gravityX() const;
    void setGravityX(float gravityX);

    float gravityY() const;
    void setGravityY(float gravityY);

signals:
    void gridWidthChanged();
    void gridHeightChanged();
    void gridDensityChanged();
    void gravityXChanged();
    void gravityYChanged();

    void ready();

protected:
    void generateGrid(int width, int height, int density);
    void clearGrid();

private:
    QVector<PhVertex*> m_vertexes;
    QVector<PhEdge*> m_edges;

    int m_gridWidth {0};
    int m_gridHeight {0};
    int m_gridDensity {0};
    float m_gravityX {0.f};
    float m_gravityY {9.8f};
};

}

#endif // PHSOLVER_H
