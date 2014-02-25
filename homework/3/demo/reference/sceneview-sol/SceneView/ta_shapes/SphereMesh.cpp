#include "SphereMesh.h"

void sphereMeshCallback(float s, float t, Vector *position, Vector *normal)
{
    s = s * M_PI - M_PI/2;
    t *= 2*M_PI;
    *normal = Vector(cosf(s) * cosf(t), -sinf(s), cosf(s) * sinf(t));
    *position = (*normal) * 0.5f;
}

SphereMesh::SphereMesh(unsigned int slices, unsigned int stacks)
{
    if (slices < 2) slices = 2;
    if (stacks < 3) stacks = 3;
    generateParameterizedSurface(sphereMeshCallback, slices, stacks, 0, 0, slices, stacks);
}

SphereMesh::~SphereMesh() {}
