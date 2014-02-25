#ifndef __CONE_MESH_H__
#define __CONE_MESH_H__

#include "ParameterizedMesh.h"

class ConeMesh : public ParameterizedMesh
{
public:
    ConeMesh(unsigned int stacks, unsigned int slices);
    virtual ~ConeMesh();
};

#endif
