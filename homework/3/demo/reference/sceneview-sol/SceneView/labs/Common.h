#undef near
#undef far

#ifndef COMMON_H
#define COMMON_H

#include "CS123Algebra.h"

struct MeshVertex{
    Vector3 position;
    Vector3 normal;
    Vector2 texCoord;

    MeshVertex():
        position(Vector3::zero()),
        normal(Vector3::zero()),
        texCoord(Vector2::zero())
    {}

    MeshVertex(Vector3 pos, Vector3 norm) :
        position(pos),
        normal(normal),
        texCoord(Vector2::zero())
    {}

    MeshVertex(const MeshVertex& copy) :
        position(copy.position),
        normal(copy.normal),
        texCoord(copy.texCoord)
    {}
};

#endif // COMMON_H
