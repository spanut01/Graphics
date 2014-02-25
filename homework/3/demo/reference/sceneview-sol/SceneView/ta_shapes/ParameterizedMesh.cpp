#include "ParameterizedMesh.h"
#include <qgl.h>

ParameterizedMesh::ParameterizedMesh()
{
}

ParameterizedMesh::~ParameterizedMesh()
{
}

void ParameterizedMesh::drawNormals(void (*drawNormal)(const Vector4 &, const Vector4 &))
{
    // Optimized version of drawNormals since there is a 1:1 vertex-to-normal ratio
    for(unsigned int i = 0; i < vertices.size(); i++)
    {
        drawNormal(
                Vector4(vertices[i].x, vertices[i].y, vertices[i].z, 1),
                Vector4(normals[i].x, normals[i].y, normals[i].z, 0));
    }
}

// void callback(float s, float t, Vector *position, Vector *normal);
// callback computes the position and normal of a parameterized surface at (s, t)
// sCount and tCount are the number of patches to subdivide the surface into
// the surface is drawn using all squares from sStart to sStop and tStart to tStop
// where 0 <= sStart < sStop <= sCount and 0 <= tStart < tStop <= tCount
void ParameterizedMesh::generateParameterizedSurface(void (*callback)(float, float, Vector *, Vector *),
    unsigned int sCount, unsigned int tCount, unsigned int sStart, unsigned int tStart,
    unsigned int sStop, unsigned int tStop)
{
    unsigned int s, t, index = vertices.size();
    float sPercent, tPercent;

    for(s = sStart, sPercent = (float)s / sCount; s <= sStop; s++, sPercent = (float)s / (float)sCount)
    {
        for(t = tStart, tPercent = (float)t / tCount; t <= tStop; t++, tPercent = (float)t / (float)tCount)
        {
            Vector vertex, normal;
            callback(sPercent, tPercent, &vertex, &normal);
            vertices.push_back(vertex);
            normals.push_back(normal);
        }
    }

    // only index across the vertices we generate above
    sCount = sStop - sStart;
    tCount = tStop - tStart;

    for(s = 0; s < sCount; s++, index++)
    {
        for(t = 0; t < tCount; t++, index++)
        {
            triangles.push_back(Triangle(index, index + 1, index + (tCount + 1)));
            triangles.push_back(Triangle(index + (tCount + 1), index + 1, index + (tCount + 1) + 1));
        }
    }
}
