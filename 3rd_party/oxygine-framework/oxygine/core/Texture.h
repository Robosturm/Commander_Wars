#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageData.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include <QOpenGLShader>

namespace oxygine
{
    class Texture;
    using spTexture = intrusive_ptr<Texture>;
    class Texture : public QObject, public Object
    {
        Q_OBJECT
        struct glPixel
        {
            qint32 format;
            qint32 type;
            bool compressed;
        };
    public:
        enum
        {
            lock_read = 0x1,
            lock_write = 0x2,
            lock_write_on_apply = 0x4,
        };
        using lock_flags = qint32;
        virtual ~Texture();
        void init(GLuint, qint32 w, qint32 h, ImageData::TextureFormat tf);
        void init(qint32 w, qint32 h, ImageData::TextureFormat tf);
        void init(const ImageData& src);

        qint32 getWidth() const
        {
            return m_width;
        }
        qint32 getHeight() const
        {
            return m_height;
        }
        GLuint getHandle() const
        {
            return m_id;
        }
        ImageData::TextureFormat getFormat() const
        {
            return m_format;
        }
        /**Disabled/Enables bilinear texture filter*/
        void setLinearFilter(quint32 filter);
        /**set wrap mode as clamp2edge or repeat (tiling)*/
        void setClamp2Edge(bool clamp2edge);
        ImageData lock(lock_flags, const Rect* src);
        void unlock();
        void updateRegion(qint32 x, qint32 y, const ImageData& data);

        timeMS getCreationTime()
        {
            return m_CreationTime;
        }
        void setCreationTime(timeMS time)
        {
            m_CreationTime = time;
        }
        void release();
        static GLuint getHighestTextureCount();

    protected:
        friend class VideoDriver;
        friend class intrusive_ptr<Texture>;
        explicit Texture() = default;
        glPixel SurfaceFormat2GL(ImageData::TextureFormat format);
        GLuint createTexture();
    protected:
        GLuint m_id{0};
        qint32 m_width{0};
        qint32 m_height{0};
        ImageData::TextureFormat m_format{ImageData::TF_UNDEFINED};
        qint32 m_lockFlags{0};
        Rect m_lockRect;
        QVector<unsigned char> m_data;
    private:
        timeMS m_CreationTime;
        static GLuint m_highestTextureCount;
    };
}
