#include "ConeMesh.h"

/*
   a = atan(1/2) = about 26.6 degrees

   |\
   |a\ sqrt(5)/2
 1 |  \
   |   \
   +----+
    0.5
*/

#define SIN_ANGLE 0.447213595499958f
#define COS_ANGLE 0.894427190999916f

void coneMeshCallbackPoint(float s, float t, Vector *position, Vector *normal)
{
    float sAngle = s * (2.0f * M_PI);
    float sSin = sinf(sAngle);
    float sCos = cosf(sAngle);
    float radius = 0.5f * t;
    *normal = Vector(COS_ANGLE * sSin, SIN_ANGLE, COS_ANGLE * sCos);
    *position = Vector(radius * sSin, 0.5f - t, radius * sCos);
}

void coneMeshCallbackBottom(float s, float t, Vector *position, Vector *normal)
{
    float sAngle = s * (-2.0f * M_PI);
    float radius = t * 0.5f;
    *position = Vector(radius * sinf(sAngle), -0.5f, radius * cosf(sAngle));
    *normal = Vector(0, -1, 0);
}

ConeMesh::ConeMesh(unsigned int stacks, unsigned int slices)
{
    if(stacks < 1) stacks = 1;
    if(slices < 3) slices = 3;

    generateParameterizedSurface(coneMeshCallbackPoint, slices, stacks, 0, 1, slices, stacks);

    unsigned int index = vertices.size();
    for(unsigned int i = 0; i < slices; i++)
    {
        float angle = ((float)i + 0.5f) * (2.0f * M_PI) / (float)slices;
        vertices.push_back(Vector(0, 0.5f, 0));
        normals.push_back(Vector(COS_ANGLE * sinf(angle), SIN_ANGLE, COS_ANGLE * cosf(angle)));
        triangles.push_back(Triangle(i * stacks, ((i + 1) % slices) * stacks, index++));
    }

    generateParameterizedSurface(coneMeshCallbackBottom, slices, stacks, 0, 0, slices, stacks);
}

ConeMesh::~ConeMesh() {}
