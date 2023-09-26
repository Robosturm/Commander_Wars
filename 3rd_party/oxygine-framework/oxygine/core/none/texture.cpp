#include "3rd_party/oxygine-framework/oxygine/core/none/texture.h"

#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include "coreengine/interpreter.h"

namespace oxygine
{
    Texture::Texture()
    {
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("Texture");
#endif
    moveToThread(GameWindow::getWindow()->getMainThread());
    }

    Texture::~Texture()
    {
        Texture::release();
    }

    void Texture::setLinearFilter(quint32 filter)
    {
    }

    void Texture::setClamp2Edge(bool clamp2edge)
    {
    }

    quint32 Texture::getHighestTextureCount()
    {
        return 0;
    }

    void Texture::release()
    {
    }

    void Texture::init(const QImage &)
    {
    }
}
