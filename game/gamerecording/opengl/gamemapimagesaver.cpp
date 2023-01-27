#include <QOpenGLFramebufferObject>

#include "game/gamerecording/gamemapimagesaver.h"
#include "game/gamemap.h"

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
        oxygine::GameWindow* window = oxygine::GameWindow::getWindow();
        qreal ratio = window->devicePixelRatio();
        auto oldSize = oxygine::Stage::getStage()->getSize();
        QSize size(GameMap::getImageSize() * pMap->getMapWidth() * pMap->getScaleX() * ratio, GameMap::getImageSize() * pMap->getMapHeight() * pMap->getScaleY() * ratio);
        QOpenGLFramebufferObject buffer(size);
        buffer.bind();
        QColor clearColor(0, 0, 255, 255);
        QRect viewport(0, 0, size.width(), size.height());
        auto viewProjection = oxygine::Stage::getViewProjectionMatrix(viewport);
        oxygine::spVideoDriver driver = oxygine::VideoDriver::instance;
        driver->setViewport(viewport);
        driver->clear(clearColor);
        oxygine::STDRenderer::instance->setViewProj(viewProjection);
        oxygine::Stage::getStage()->setSize(viewport.width(), viewport.height());
        oxygine::RenderState rs;
        QRect clip(0, 0, viewport.width(), viewport.height());
        rs.clip = &clip;
        auto orgPos = pMap->getPosition();
        pMap->setPosition(0, 0);
        menu.getCursor()->setVisible(false);
        pMap->render(rs);
        oxygine::STDRenderer::getCurrent()->flush();
        oxygine::Material::null->apply();
        pMap->setPosition(orgPos);
        menu.getCursor()->setVisible(true);
        auto img = buffer.toImage();
        img.save(filename);
        buffer.release();
        oxygine::Stage::getStage()->setSize(oldSize);
    }
}
