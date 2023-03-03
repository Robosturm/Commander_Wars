#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

namespace oxygine
{
    class VStyleActor;
    using spVStyleActor = intrusive_ptr<VStyleActor>;
    class VStyleActor : public Actor
    {
    public:
        explicit VStyleActor();
        virtual ~VStyleActor() = default;
        const QColor& getColor() const
        {
#ifdef GRAPHICSUPPORT
            return m_color;
#else
        return m_dummyColor;
#endif
        }
        const QColor& getAddColor() const
        {
    #ifdef GRAPHICSUPPORT
            return m_mat->m_addColor;
    #else
            return m_dummyColor;
    #endif
        }

        void setColor(const QColor& color);
        void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        void setBlendMode(VideoDriver::blend_mode mode);
        void setAddColor(const QColor& color);
        void setAddColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0);
        virtual void setEnabled(bool value) override;
        using TweenColor = Property<QColor, const QColor&, VStyleActor, &VStyleActor::getColor, &VStyleActor::setColor>;
        using TweenAddColor = Property<QColor, const QColor&, VStyleActor, &VStyleActor::getAddColor, &VStyleActor::setAddColor>;
        QColor getDisableColor() const
        {
    #ifdef GRAPHICSUPPORT
            return m_disableColor;
    #else
            return m_dummyColor;
    #endif
        }
        void setDisableColor(const QColor &value);

#ifdef GRAPHICSUPPORT
        inline spMaterial & getMaterial()
        {
            return m_mat;
        }
#endif
    protected:
        virtual void matChanged() {}
        void setMaterial(spMaterial & mat);
    private:
        void changeAddColor(const QColor& color);
    protected:
#ifdef GRAPHICSUPPORT
        QColor m_color{Qt::white};
        QColor m_disableColor{75, 75, 75, 0};
        spMaterial m_mat;
#else
        static QColor m_dummyColor;
#endif

    };

}
