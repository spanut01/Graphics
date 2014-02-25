#include "LabCanvas.h"

#include <QMenuBar>
#include <QPushButton>
#include <QToolBar>
#include <iostream>

// Determines the spacing between buttons on the Lab Canvas:
#define LABCANVAS_BUTTON_SPACING  4
#define LABCANVAS_BUTTON_WIDTH   110
#define LABCANVAS_BUTTON_HEIGHT  24


LabCanvas::LabCanvas(QWidget *parent, QMenuBar *menu) : SupportCanvas3D(parent) {

    // default render settings
    m_renderTechnique = RENDER_SHADED;
    m_lightingEnabled = true;

    m_menuOffset = LABCANVAS_BUTTON_SPACING;

    makeRenderTechniqueButton("Points",     RENDER_POINT);
    makeRenderTechniqueButton("Wireframe",  RENDER_WIREFRAME);
    makeRenderTechniqueButton("Shaded",     RENDER_SHADED);

    QPushButton* b = makeButton("Toggle Lighting");
    b->setCheckable(true);
    b->setChecked(m_lightingEnabled);
    connect(b, SIGNAL(clicked()), this, SLOT(toggleLights()));
}

LabCanvas::~LabCanvas() {

}

QPushButton* LabCanvas::makeButton(QString title) {

    //  Why don't we need to delete this QPushButton? We are setting the
    //  "parent" of the button to "this" - hence, when "this" is deleted,
    //  Qt will delete the button for us. How convenient!
    //
    //  http://doc.trolltech.com/4.5/objecttrees.html

    QPushButton *push = new QPushButton(title, this);

    QRect dr;
    dr.setLeft(m_menuOffset);
    dr.setTop(LABCANVAS_BUTTON_SPACING);
    dr.setWidth(LABCANVAS_BUTTON_WIDTH);
    dr.setHeight(LABCANVAS_BUTTON_HEIGHT);
    push->setGeometry(dr);
    m_menuOffset += dr.width();

    m_menuOffset += LABCANVAS_BUTTON_SPACING;
    return push;
}

void LabCanvas::makeRenderTechniqueButton(QString title, int renderTechnique) {
    QPushButton* push = makeButton(title);
    push->setProperty("data", renderTechnique);
    connect(push, SIGNAL(clicked()), this, SLOT(pickRenderTechnique()));
}

void LabCanvas::pickRenderTechnique() {
    QPushButton* push = (QPushButton*)QObject::sender();
    m_renderTechnique = (RenderTechnique)push->property("data").toInt();
    std::cout << "Picked render technique: " << m_renderTechnique << std::endl;
    applyRenderSettings();
    repaint();
}

void LabCanvas::toggleLights() {
    this->m_lightingEnabled = !this->m_lightingEnabled;
    ((QPushButton*)QObject::sender())->setChecked(this->m_lightingEnabled);
    applyRenderSettings();
    repaint();
}

void LabCanvas::makeSMMenu(QString title, int shademodel) {

}

void LabCanvas::pickSMMenu() {

}

void LabCanvas::settingsChanged()
{
    // TODO: Fill this in if you like.
}
