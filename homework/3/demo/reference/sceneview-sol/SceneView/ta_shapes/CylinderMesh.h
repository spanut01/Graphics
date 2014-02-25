#ifndef __CYLINDER_MESH_H__
#define __CYLINDER_MESH_H__

#include "ParameterizedMesh.h"

class CylinderMesh : public ParameterizedMesh
{
public:
    CylinderMesh(unsigned int stacks, unsigned int slices);
    virtual ~CylinderMesh();
};

#endif
