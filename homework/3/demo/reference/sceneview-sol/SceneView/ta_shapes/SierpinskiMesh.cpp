#include "SierpinskiMesh.h"
#include <qgl.h>

#ifndef M_SQRT1_2
#define M_SQRT1_2 7.0710678118654752440E-1
#endif

Vector a = Vector(-M_SQRT1_2, 0, -0.5f);
Vector b = Vector(M_SQRT1_2, 0, -0.5f);
Vector c = Vector(0, -M_SQRT1_2, 0.5f);
Vector d = Vector(0, M_SQRT1_2, 0.5f);

SierpinskiMesh::SierpinskiMesh(unsigned int depth)
{
        // Parameter must be <= 8 due to large size\n");
        if(--depth > 7)
                depth = 7;

    normals.push_back((b - a).Cross(c - a).Unit());
    normals.push_back((d - a).Cross(b - a).Unit());
    normals.push_back((c - a).Cross(d - a).Unit());
    normals.push_back((d - b).Cross(c - b).Unit());
    construct(a, b, c, d, depth);
}

SierpinskiMesh::~SierpinskiMesh()
{
}

void SierpinskiMesh::construct(const Vector &a, const Vector &b, const Vector &c, const Vector &d, unsigned int depth)
{
    if(depth--)
    {
        Vector ab = (a + b) * 0.5f;
        Vector ac = (a + c) * 0.5f;
        Vector ad = (a + d) * 0.5f;
        Vector bc = (b + c) * 0.5f;
        Vector bd = (b + d) * 0.5f;
        Vector cd = (c + d) * 0.5f;
        construct(a, ab, ac, ad, depth);
        construct(ab, b, bc, bd, depth);
        construct(ac, bc, c, cd, depth);
        construct(ad, bd, cd, d, depth);
    }
    else
    {
        unsigned int index = vertices.size();
        vertices.push_back(a);
        vertices.push_back(b);
        vertices.push_back(c);
        vertices.push_back(d);
        triangles.push_back(Triangle(index, 0, index + 1, 0, index + 2, 0));
        triangles.push_back(Triangle(index, 1, index + 3, 1, index + 1, 1));
        triangles.push_back(Triangle(index, 2, index + 2, 2, index + 3, 2));
        triangles.push_back(Triangle(index + 1, 3, index + 3, 3, index + 2, 3));
    }
}
