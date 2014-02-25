#include "ObjMesh.h"
#include "glm.cpp"
#include <QMessageBox>

ObjMesh::ObjMesh(const char *file)
{
    m_matrix = Matrix4x4::identity();
    m_model = glmReadOBJ(file);

    if (m_model == NULL)
        QMessageBox::critical(NULL, "Error", "Could not load obj file \"" + QString(file) + "\"");
}

ObjMesh::~ObjMesh()
{
    glmDelete(m_model);
}

void ObjMesh::transformByMatrix(const Matrix4x4 &matrix)
{
    m_matrix *= matrix;
}

void ObjMesh::draw()
{
    // Save and restore the material properties so we don't mess up the next scene (and the rest of this one)
    float oldAmbient[4];
    float oldDiffuse[4];
    float oldSpecular[4];
    float oldEmission[4];
    float oldShininess;

    glGetMaterialfv(GL_FRONT, GL_AMBIENT, oldAmbient);
    glGetMaterialfv(GL_FRONT, GL_DIFFUSE, oldDiffuse);
    glGetMaterialfv(GL_FRONT, GL_SPECULAR, oldSpecular);
    glGetMaterialfv(GL_FRONT, GL_EMISSION, oldEmission);
    glGetMaterialfv(GL_FRONT, GL_SHININESS, &oldShininess);

    // Draw the model with its transformation matrix (need GL_NORMALIZE to normalize the model normals in case m_matrix has a scale transformation)
    Matrix4x4 bufferM = m_matrix;
    double *buffer = bufferM.getTranspose().data;
    glPushMatrix();
    glMultMatrixd(buffer);
    glEnable(GL_NORMALIZE);
    glmDraw(m_model, GLM_SMOOTH | GLM_MATERIAL);
    glDisable(GL_NORMALIZE);
    glPopMatrix();

    // cout << "oldAmbient = " << oldAmbient[0] << ", " << oldAmbient[1] << ", " << oldAmbient[2] << ", " << oldAmbient[3] << endl;
    // cout << "oldDiffuse = " << oldDiffuse[0] << ", " << oldDiffuse[1] << ", " << oldDiffuse[2] << ", " << oldDiffuse[3] << endl;
    // cout << "oldSpecular = " << oldSpecular[0] << ", " << oldSpecular[1] << ", " << oldSpecular[2] << ", " << oldSpecular[3] << endl;
    // cout << "oldEmission = " << oldEmission[0] << ", " << oldEmission[1] << ", " << oldEmission[2] << ", " << oldEmission[3] << endl;
    // cout << "oldShininess = " << oldShininess << endl;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, oldAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, oldDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, oldSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, oldEmission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, oldShininess);
}
