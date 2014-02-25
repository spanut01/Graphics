#ifndef __CUBE_MESH_H__
#define __CUBE_MESH_H__

#include "ParameterizedMesh.h"

class CubeMesh : public ParameterizedMesh
{
public:
    CubeMesh(unsigned int squaresPerEdge);
    virtual ~CubeMesh();
};

#endif
