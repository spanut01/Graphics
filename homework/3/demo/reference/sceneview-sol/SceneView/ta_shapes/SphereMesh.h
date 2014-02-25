#ifndef __SPHERE_MESH_H__
#define __SPHERE_MESH_H__

#include "ParameterizedMesh.h"

class SphereMesh : public ParameterizedMesh
{
public:
        SphereMesh(unsigned int slices, unsigned int stacks);
    virtual ~SphereMesh();
};

#endif
