#include "Mesh.h"
#include <qgl.h>

Mesh::Mesh()
{
    cachedDisplayList = 0;
}

Mesh::~Mesh()
{
    glDeleteLists(1, cachedDisplayList);
}

void Mesh::transformByMatrix(const Matrix4x4 &matrix)
{
        glDeleteLists(1, cachedDisplayList);
        cachedDisplayList = 0;

        for(unsigned int i = 0; i < vertices.size(); i++)
        {
                Vector4 point = Vector4(vertices[i].x, vertices[i].y, vertices[i].z, 1);
                point = matrix * point;
                vertices[i] = Vector(point.data[0], point.data[1], point.data[2]);
        }

        // normals must be transformed with the transpose of the inverse of matrix
        // http://tog.acm.org/resources/RTNews/html/rtnews1a.html#art4
        Matrix4x4 adjunctMatrix = matrix.getInverse().getTranspose();

        for(unsigned int i = 0; i < normals.size(); i++)
        {
                Vector4 point = Vector4(normals[i].x, normals[i].y, normals[i].z, 0);
                point = adjunctMatrix * point;
                normals[i] = Vector(point.data[0], point.data[1], point.data[2]).Unit();
        }
}

void Mesh::drawCached()
{
    if(!cachedDisplayList)
    {
        cachedDisplayList = glGenLists(1);
        glNewList(cachedDisplayList, GL_COMPILE_AND_EXECUTE);
        draw();
        glEndList();
    }
    else glCallList(cachedDisplayList);
}

void Mesh::draw()
{
    glBegin(GL_TRIANGLES);
    for(unsigned int i = 0; i < triangles.size(); i++)
    {
        Triangle &triangle = triangles[i];

        glNormal3fv(&normals[triangle.a_normal].x);
        glVertex3fv(&vertices[triangle.a_vertex].x);

        glNormal3fv(&normals[triangle.b_normal].x);
        glVertex3fv(&vertices[triangle.b_vertex].x);

        glNormal3fv(&normals[triangle.c_normal].x);
        glVertex3fv(&vertices[triangle.c_vertex].x);
    }
    glEnd();
}

void Mesh::drawNormals(void (*drawNormal)(const Vector4 &, const Vector4 &))
{
    for(unsigned int i = 0; i < triangles.size(); i++)
    {
            Triangle &triangle = triangles[i];

            Vector &v1 = vertices[triangle.a_vertex];
            Vector &n1 = normals[triangle.a_normal];
            drawNormal(Vector4(v1.x, v1.y, v1.z, 1), Vector4(n1.x, n1.y, n1.z, 0));

            Vector &v2 = vertices[triangle.b_vertex];
            Vector &n2 = normals[triangle.b_normal];
            drawNormal(Vector4(v2.x, v2.y, v2.z, 1), Vector4(n2.x, n2.y, n2.z, 0));

            Vector &v3 = vertices[triangle.c_vertex];
            Vector &n3 = normals[triangle.c_normal];
            drawNormal(Vector4(v3.x, v3.y, v3.z, 1), Vector4(n3.x, n3.y, n3.z, 0));
    }
}
