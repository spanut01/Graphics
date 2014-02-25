#include "CubeMesh.h"

void cubeMeshCallbackXneg(float s, float t, Vector *position, Vector *normal)
{
    *position = Vector(-0.5f, s - 0.5f, t - 0.5f);
    *normal = Vector(-1, 0, 0);
}

void cubeMeshCallbackXpos(float s, float t, Vector *position, Vector *normal)
{
    *position = Vector(0.5f, 0.5f - s, t - 0.5f);
    *normal = Vector(+1, 0, 0);
}

void cubeMeshCallbackYneg(float s, float t, Vector *position, Vector *normal)
{
    *position = Vector(0.5f - s, -0.5f, t - 0.5f);
    *normal = Vector(0, -1, 0);
}

void cubeMeshCallbackYpos(float s, float t, Vector *position, Vector *normal)
{
    *position = Vector(s - 0.5f, 0.5f, t - 0.5f);
    *normal = Vector(0, +1, 0);
}

void cubeMeshCallbackZneg(float s, float t, Vector *position, Vector *normal)
{
    *position = Vector(s - 0.5f, t - 0.5f, -0.5f);
    *normal = Vector(0, 0, -1);
}

void cubeMeshCallbackZpos(float s, float t, Vector *position, Vector *normal)
{
    *position = Vector(0.5f - s, t - 0.5f, 0.5f);
    *normal = Vector(0, 0, +1);
}

CubeMesh::CubeMesh(unsigned int squaresPerEdge)
{
    if(squaresPerEdge < 1) squaresPerEdge = 1;

    generateParameterizedSurface(cubeMeshCallbackXneg, squaresPerEdge, squaresPerEdge, 0, 0, squaresPerEdge, squaresPerEdge);
    generateParameterizedSurface(cubeMeshCallbackXpos, squaresPerEdge, squaresPerEdge, 0, 0, squaresPerEdge, squaresPerEdge);
    generateParameterizedSurface(cubeMeshCallbackYneg, squaresPerEdge, squaresPerEdge, 0, 0, squaresPerEdge, squaresPerEdge);
    generateParameterizedSurface(cubeMeshCallbackYpos, squaresPerEdge, squaresPerEdge, 0, 0, squaresPerEdge, squaresPerEdge);
    generateParameterizedSurface(cubeMeshCallbackZneg, squaresPerEdge, squaresPerEdge, 0, 0, squaresPerEdge, squaresPerEdge);
    generateParameterizedSurface(cubeMeshCallbackZpos, squaresPerEdge, squaresPerEdge, 0, 0, squaresPerEdge, squaresPerEdge);
}

CubeMesh::~CubeMesh() {}
