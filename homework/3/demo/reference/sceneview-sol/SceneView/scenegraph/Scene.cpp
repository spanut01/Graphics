#include "Scene.h"
#include "Camera.h"
#include "CS123SceneData.h"
#include "CS123ISceneParser.h"
#include <QtGlobal>

// begin ta code
CS123SceneFileMap *copyFileMap(CS123SceneFileMap *original)
{
    if (original == NULL) return NULL;
    CS123SceneFileMap *copy = new CS123SceneFileMap;
    *copy = *original;
    return copy;
}

void recursiveParseNode(Scene *sceneToFill, CS123SceneNode *node, Matrix4x4 compositeMatrix)
{
    foreach (CS123SceneTransformation *transform, node->transformations)
    {
        switch (transform->type)
        {
        case TRANSFORMATION_TRANSLATE:
            compositeMatrix = compositeMatrix * getTransMat(transform->translate);
            break;

        case TRANSFORMATION_ROTATE:
            compositeMatrix = compositeMatrix * getRotMat(Vector4(), transform->rotate, transform->angle);
            break;

        case TRANSFORMATION_SCALE:
            compositeMatrix = compositeMatrix * getScaleMat(transform->scale);
            break;

        case TRANSFORMATION_MATRIX:
            compositeMatrix = compositeMatrix * transform->matrix;
            break;
        }
    }

    // Tell the subclass about all the primitives
    foreach (CS123ScenePrimitive *primitive, node->primitives)
        sceneToFill->addPrimitive(*primitive, compositeMatrix);

    // Recursively parse the child nodes
    foreach (CS123SceneNode *child, node->children)
        recursiveParseNode(sceneToFill, child, compositeMatrix);
}

void Scene::invokeAddObjects(Scene* target) const
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = { 1, 1, 1, 1};
    target->setGlobal(global);

    foreach (CS123SceneLightData l, m_lights) {
        target->addLight(l);
    }
    foreach (const PrimitiveAndMatrix &p, m_primitivesAndMatrices) {
        target->addPrimitive(p.primitive, p.matrix);
    }
    target->finishParsing();
}

void operator *= (CS123SceneColor &color, float scale)
{
    color.r *= scale;
    color.g *= scale;
    color.b *= scale;
    color.a *= scale;
}
// end ta code

Scene::Scene() //: m_camera(NULL)
{

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser)
{
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and finishParsing()
    // begin ta code
    CS123SceneNode *root = parser->getRootNode();

    // Tell the subclass about the global data
    CS123SceneGlobalData global;
    if (parser->getGlobalData(global))
        sceneToFill->setGlobal(global);

    // Tell the subclass about all the lights
    CS123SceneLightData lightData;
    for (int i = 0; i < parser->getNumLights(); i++)
        if (parser->getLightData(i, lightData))
            sceneToFill->addLight(lightData);

    // Parse the root (the world transformation starts off as the identity transform)
    if (root)
        recursiveParseNode(sceneToFill, root, Matrix4x4::identity());

    sceneToFill->finishParsing();

    // end ta code
}

//void Scene::setCamera(Camera *newCamera)
//{
//    // Do not delete the old m_camera, it is owned by SupportCanvas3D
//    m_camera = newCamera;
//}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const Matrix4x4 &matrix)
{
    // begin ta code
    CS123ScenePrimitive copy;
    copy.type = scenePrimitive.type;
    copy.meshfile = scenePrimitive.meshfile;
    copy.material = scenePrimitive.material;
    copy.material.cAmbient.r *= m_global.ka;
    copy.material.cAmbient.g *= m_global.ka;
    copy.material.cAmbient.b *= m_global.ka;
    copy.material.cDiffuse.r *= m_global.kd;
    copy.material.cDiffuse.g *= m_global.kd;
    copy.material.cDiffuse.b *= m_global.kd;
    copy.material.cSpecular.r *= m_global.ks;
    copy.material.cSpecular.g *= m_global.ks;
    copy.material.cSpecular.b *= m_global.ks;
    copy.material.cReflective.r *= m_global.ks;
    copy.material.cReflective.g *= m_global.ks;
    copy.material.cReflective.b *= m_global.ks;
    copy.material.cTransparent.r *= m_global.kt;
    copy.material.cTransparent.g *= m_global.kt;
    copy.material.cTransparent.b *= m_global.kt; // POST about this
    copy.material.textureMap = copyFileMap(scenePrimitive.material.textureMap);
    copy.material.bumpMap = copyFileMap(scenePrimitive.material.bumpMap);
    m_primitivesAndMatrices += PrimitiveAndMatrix(copy, matrix);
    // end ta code
}

void Scene::addLight(const CS123SceneLightData &sceneLight)
{
    // begin ta code
    m_lights += sceneLight;
    // end ta code
}

void Scene::setGlobal(const CS123SceneGlobalData &global)
{
    // begin ta code
    m_global = global;
    // end ta code
}

// begin ta code
void Scene::finishParsing()
{
}
// end ta code
