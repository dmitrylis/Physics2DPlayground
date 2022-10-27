#ifndef PHEDGE_H
#define PHEDGE_H

#include "PhVertex.h"

#include <cstddef>

namespace ph {

class PhEdge
{
public:
    explicit PhEdge(PhVertex* vertexA, PhVertex* vertexB);

    PhEdge(nullptr_t, nullptr_t) = delete;
    PhEdge(PhVertex*, nullptr_t) = delete;
    PhEdge(nullptr_t, PhVertex*) = delete;

    PhVertex* vertexA() const;
    PhVertex* vertexB() const;
    float initialLength() const;

    QVector2D differenceVector() const;

private:
    PhVertex* m_vertexA {nullptr};
    PhVertex* m_vertexB {nullptr};
    float m_initialLength {0.f};
};

}

#endif // PHEDGE_H
