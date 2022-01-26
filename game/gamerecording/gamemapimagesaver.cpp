#include <QOpenGLFramebufferObject>

#include "game/gamerecording/gamemapimagesaver.h"

#include "game/gamemap.h"

#include "menue/ingamemenue.h"

void GamemapImageSaver::saveMapAsImage(QString filename)
{
    
    spInGameMenue pMenu(InGameMenue::getMenuInstance());
    if (m_pMap != nullptr &&
        pMenu.get() != nullptr)
    {
        QSize size(GameMap::getImageSize() * m_pMap->getMapWidth() * m_pMap->getScaleX(), GameMap::getImageSize() * m_pMap->getMapHeight() * m_pMap->getScaleY());
        QOpenGLFramebufferObject buffer(size);
        buffer.bind();
        QColor clearColor(0, 0, 255, 255);
        oxygine::Rect viewport(oxygine::Point(0, 0), oxygine::Point(size.width(), size.height()));
        auto viewProjection = oxygine::Stage::getViewProjectionMatrix(viewport);
        oxygine::spVideoDriver driver = oxygine::VideoDriver::instance;
        driver->setViewport(viewport);
        driver->clear(clearColor);
        oxygine::STDRenderer::instance->setViewProj(viewProjection);
        oxygine::Stage::getStage()->setSize(viewport.getWidth(), viewport.getHeight());
        oxygine::RenderState rs;
        oxygine::RectF clip(0.0f, 0.0f, viewport.getWidth(), viewport.getHeight());
        rs.clip = &clip;        
        auto orgPos = m_pMap->getPosition();
        m_pMap->setPosition(0, 0);
        pMenu->getCursor()->setVisible(false);
        m_pMap->render(rs);
        oxygine::STDRenderer::getCurrent()->flush();
        oxygine::Material::null->apply();
        m_pMap->setPosition(orgPos);
        pMenu->getCursor()->setVisible(true);
        auto img = buffer.toImage();
        img.save(filename);
        buffer.release();
    }
}
