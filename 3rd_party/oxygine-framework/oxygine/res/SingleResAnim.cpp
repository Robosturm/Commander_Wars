#include "3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/core/texture.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include <QApplication>

#include "spritingsupport/spritecreator.h"

namespace oxygine
{

    SingleResAnim::SingleResAnim()
    {
        setObjectName("SingleResAnim");
    }

    SingleResAnim::~SingleResAnim()
    {
        m_frames.clear();
        m_texture = nullptr;
        // if (!oxygine::GameWindow::getWindow()->getShuttingDown())
        // {
        //     emit oxygine::GameWindow::getWindow()->sigWaitOnRelease();
        // }
    }

    void SingleResAnim::init(QString file, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder)
    {
        ResAnim::init(file, columns, rows, scaleFactor, addTransparentBorder);
    }

    void SingleResAnim::init(QImage & image, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder,
                             bool clamp2Edge, quint32 linearFilter)
    {
        SpriteCreator::convertToRgba(image);
        m_scaleFactor = scaleFactor;
        m_frames.clear();
        m_texture = nullptr;
        m_texture = VideoDriver::instance->createTexture();
        m_texture->init(image);
        m_texture->setClamp2Edge(clamp2Edge);
        m_texture->setLinearFilter(linearFilter);
        Point originalSize = Point(image.width(), image.height());
        ResAnim::init(m_texture, image.size(), columns, rows, scaleFactor, addTransparentBorder);
    }

    spTexture SingleResAnim::getTexture() const
    {
        return m_texture;
    }

}
