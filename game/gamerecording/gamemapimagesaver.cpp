#include <QOpenGLFramebufferObject>

#include "game/gamerecording/gamemapimagesaver.h"

#include "game/gamemap.h"

#include "menue/ingamemenue.h"

void GamemapImageSaver::saveMapAsImage(QString filename)
{
    spGameMap pMap = GameMap::getInstance();
    spInGameMenue pMenu(InGameMenue::getMenuInstance());
    if (pMap.get() != nullptr &&
        pMenu.get() != nullptr)
    {
        constexpr qint32 extraBorder = 5;
        QSize size(GameMap::getImageSize() * pMap->getMapWidth() * pMap->getScaleX() + extraBorder, GameMap::getImageSize() * pMap->getMapHeight() * pMap->getScaleY() + extraBorder);
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
        auto orgPos = pMap->getPosition();
        pMap->setPosition(0, 0);
        pMenu->getCursor()->setVisible(false);
        pMap->render(rs);
        oxygine::STDRenderer::getCurrent()->flush();
        oxygine::Material::null->apply();
        pMap->setPosition(orgPos);
        pMenu->getCursor()->setVisible(true);
        auto img = buffer.toImage().copy(QRect(0, 0, size.width() - extraBorder, size.height() - extraBorder));
        img.save(filename);
        buffer.release();
    }
}
