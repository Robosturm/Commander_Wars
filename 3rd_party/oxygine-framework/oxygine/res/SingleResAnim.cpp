#include "3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "texture.h"

#include "spritingsupport/spritecreator.h"

namespace oxygine
{

    SingleResAnim::~SingleResAnim()
    {
        m_frames.clear();
        m_texture.free();
    }

    void SingleResAnim::init(const QString & file, qint32 columns, qint32 rows, float scaleFactor)
    {
        ResAnim::init(file, columns, rows, scaleFactor);
    }

    void SingleResAnim::init(QImage & image, qint32 columns, qint32 rows, float scaleFactor,
                             bool clamp2Edge, quint32 linearFilter)
    {

        SpriteCreator::convertToRgba(image);
        m_scaleFactor = scaleFactor;
        m_frames.clear();
        m_texture.free();
        m_texture = VideoDriver::instance->createTexture();
        m_texture->init(image);
        m_texture->setClamp2Edge(clamp2Edge);
        m_texture->setLinearFilter(linearFilter);
        ResAnim::init(m_texture, image.size(), columns, rows, scaleFactor);
    }

    spTexture SingleResAnim::getTexture() const
    {
        return m_texture;
    }

}
