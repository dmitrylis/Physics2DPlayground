#ifndef PHRENDERER_H
#define PHRENDERER_H

#include <QQuickPaintedItem>
#include <QBasicTimer>

#include "PhSolver.h"

namespace ph {

class PhRenderer : public QQuickPaintedItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(PhRenderer)

    // "solver" should be a REQUIRED property, but Qt generates a strange warning
    Q_PROPERTY(PhSolver* solver READ solver WRITE setSolver NOTIFY solverChanged /*REQUIRED*/)
    Q_PROPERTY(bool timerActive READ timerActive WRITE setTimerActive NOTIFY timerActiveChanged)

public:
    explicit PhRenderer(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

    PhSolver *solver() const;
    void setSolver(PhSolver *solver);

    bool timerActive() const;
    void setTimerActive(bool timerActive);

    void naiveUpdate();
    void smartUpdate();

protected:
    void timerEvent(QTimerEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void startExecution();

signals:
    void solverChanged();
    void timerActiveChanged();

private:
    PhSolver* m_solver {nullptr};

    QBasicTimer m_timer;
    bool m_timerActive {false};

    // for drag & drop vertex
    PhVertex* m_currentVertex {nullptr};
    bool m_wasLocked {false};

    // for moving space
    QVector2D m_offset {100.f, 100.f};
    QVector2D m_pressOffset;

    // for zooming
    float m_zoom {1.f};
};

}

#endif // PHRENDERER_H
