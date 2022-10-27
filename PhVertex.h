#ifndef PHVERTEX_H
#define PHVERTEX_H

#include <QVector2D>

namespace ph {

class PhVertex
{
public:
    explicit PhVertex(const QVector2D& position, bool locked);
    explicit PhVertex(float x, float y, bool locked);

    QVector2D& position();
    QVector2D& prevPosition();
    bool& locked();

private:
    QVector2D m_position;
    QVector2D m_prevPosition;
    bool m_locked {false};
};

}

#endif // PHVERTEX_H
