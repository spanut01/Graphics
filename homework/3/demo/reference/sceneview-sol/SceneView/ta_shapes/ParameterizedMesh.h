#ifndef __PARAMETERIZED_MESH_H__
#define __PARAMETERIZED_MESH_H__

#include "Mesh.h"

class ParameterizedMesh : public Mesh
{
public:
    ParameterizedMesh();
    virtual ~ParameterizedMesh();

        void drawNormals(void (*drawNormal)(const Vector4 &, const Vector4 &));

protected:
    // void callback(float s, float t, Vector *position, Vector *normal);
    // callback computes the position and normal of a parameterized surface at (s, t)
    // sCount and tCount are the number of patches to subdivide the surface into
    // the surface is drawn using all squares from sStart to sStop and tStart to tStop
    // where 0 <= sStart < sStop <= sCount and 0 <= tStart < tStop <= tCount
    void generateParameterizedSurface(void (*callback)(float, float, Vector *, Vector *),
        unsigned int sCount, unsigned int tCount, unsigned int sStart, unsigned int tStart,
        unsigned int sStop, unsigned int tStop);
};

#endif
