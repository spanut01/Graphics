#include "CylinderMesh.h"

void cylinderMeshCallbackBarrel(float s, float t, Vector *position, Vector *normal)
{
    float sAngle = s * (2.0f * M_PI);
    *normal = Vector(sinf(sAngle), 0.0f, cosf(sAngle));
    *position = Vector(0.5f * normal->x, 0.5f - t, 0.5f * normal->z);
}

void cylinderMeshCallbackTop(float s, float t, Vector *position, Vector *normal)
{
    float sAngle = s * (2.0f * M_PI);
    float radius = t * 0.5f;
    *position = Vector(radius * sinf(sAngle), 0.5f, radius * cosf(sAngle));
    *normal = Vector(0, +1, 0);
}

void cylinderMeshCallbackBottom(float s, float t, Vector *position, Vector *normal)
{
    float sAngle = s * (-2.0f * M_PI);
    float radius = t * 0.5f;
    *position = Vector(radius * sinf(sAngle), -0.5f, radius * cosf(sAngle));
    *normal = Vector(0, -1, 0);
}

CylinderMesh::CylinderMesh(unsigned int stacks, unsigned int slices)
{
    if(stacks < 1) stacks = 1;
    if(slices < 3) slices = 3;

    generateParameterizedSurface(cylinderMeshCallbackBarrel, slices, stacks, 0, 0, slices, stacks);
    generateParameterizedSurface(cylinderMeshCallbackTop,    slices, stacks, 0, 0, slices, stacks);
    generateParameterizedSurface(cylinderMeshCallbackBottom, slices, stacks, 0, 0, slices, stacks);
}

CylinderMesh::~CylinderMesh() {}
