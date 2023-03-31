#include <QOpenGLFramebufferObject>

#include "game/gamerecording/gamemapimagesaver.h"
#include "game/gamemap.h"

#include "objects/minimap.h"

#include "menue/basegamemenu.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"

void GamemapImageSaver::saveMapAsImage(QString filename, BaseGamemenu& menu)
{
    auto* pMap = menu.getMap();
    if (pMap != nullptr)
    {
        // store back up
        oxygine::spVideoDriver driver = oxygine::VideoDriver::instance;
        auto orgPos = pMap->getPosition();
        pMap->setPosition(0, 0);
        menu.getCursor()->setVisible(false);

        // render
        QSize size(GameMap::getImageSize() * pMap->getMapWidth() * pMap->getScaleX(), GameMap::getImageSize() * pMap->getMapHeight() * pMap->getScaleY());
        QOpenGLFramebufferObject buffer(size);
        if (buffer.bind())
        {
            QColor clearColor(0, 0, 255, 255);
            oxygine::GameWindow* window = oxygine::GameWindow::getWindow();
            qreal ratio = window->devicePixelRatio();
            QRectF viewport(0, 0, size.width() / ratio, size.height() / ratio);
            auto viewProjection = oxygine::Stage::getViewProjectionMatrix(viewport);
            oxygine::rsCache().reset();
            driver->setViewport(viewport);
            driver->clear(clearColor);
            oxygine::STDRenderer::instance->setViewProj(viewProjection);
            oxygine::RenderState rs;
            rs.clip = QRect(0, 0, viewport.width(), viewport.height());
            rs.transform.setMatrix(1 / ratio, 0, 0,
                                   0, 1 / ratio, 0,
                                   0, 0, 1);
            pMap->render(rs);
            oxygine::STDRenderer::getCurrent()->flush();
            oxygine::Material::null->apply();
            auto img = buffer.toImage();
            img.save(filename);

            // clean up
            pMap->setPosition(orgPos);
            menu.getCursor()->setVisible(true);
            buffer.release();
        }
    }
}

void GamemapImageSaver::saveMapAsImage(Minimap* pMinimap, QImage & img)
{
    if (pMinimap != nullptr)
    {
        // store back up
        oxygine::spVideoDriver driver = oxygine::VideoDriver::instance;
        auto orgScale = pMinimap->getScale();
        auto orgPos = pMinimap->getPosition();
        pMinimap->setPosition(0, 0);
        pMinimap->setScale(2.0f);

        // render
        QSize size(pMinimap->getScaledWidth(), pMinimap->getScaledHeight());
        QOpenGLFramebufferObject buffer(size);
        if (buffer.bind())
        {
            oxygine::GameWindow* window = oxygine::GameWindow::getWindow();
            qreal ratio = window->devicePixelRatio();
            QColor clearColor(0, 0, 255, 255);
            QRectF viewport(0, 0, size.width() / ratio, size.height() / ratio);
            auto viewProjection = oxygine::Stage::getViewProjectionMatrix(viewport);
            oxygine::rsCache().reset();
            driver->setViewport(viewport);
            driver->clear(clearColor);
            oxygine::STDRenderer::instance->setViewProj(viewProjection);
            oxygine::RenderState rs;
            rs.transform.setMatrix(1 / ratio, 0, 0,
                                   0, 1 / ratio, 0,
                                   0, 0, 1);
            rs.clip = QRect(0, 0, viewport.width(), viewport.height());
            pMinimap->render(rs);
            oxygine::STDRenderer::getCurrent()->flush();
            oxygine::Material::null->apply();
            img = buffer.toImage();

            // clean up
            buffer.release();
            pMinimap->setPosition(orgPos);
            pMinimap->setScale(orgScale);
        }
    }
}
