#include <QOpenGLFramebufferObject>

#include "game/gamerecording/gamemapimagesaver.h"
#include "game/gamemap.h"

#include "menue/basegamemenu.h"

#include "3rd_party/oxygine-framework/oxygine-framework.h"

void GamemapImageSaver::saveMapAsImage(QString filename)
{
    
    spBaseGamemenu pMenu(BaseGamemenu::getInstance());

    if (pMenu.get() != nullptr)
    {
        GameMap* pMap = pMenu->getMap();
        if (pMap != nullptr)
        {
            oxygine::GameWindow* window = oxygine::GameWindow::getWindow();
            qreal ratio = window->devicePixelRatio();
            QSize size(GameMap::getImageSize() * pMap->getMapWidth() * pMap->getScaleX() * ratio, GameMap::getImageSize() * pMap->getMapHeight() * pMap->getScaleY() * ratio);
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
            auto img = buffer.toImage();
            img.save(filename);
            buffer.release();
        }
    }
}
