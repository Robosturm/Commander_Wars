#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

namespace oxygine
{
    class VisualStyle
    {
    public:
        explicit VisualStyle() = default;
        virtual ~VisualStyle() = default;
        const QColor& getColor() const
        {
            return m_color;
        }
        VideoDriver::blend_mode getBlendMode() const
        {
            return m_blend;
        }
        void setColor(const QColor& color);
        void setBlendMode(VideoDriver::blend_mode mode);

    protected:
        QColor m_color{Qt::white};
        VideoDriver::blend_mode m_blend{VideoDriver::blend_premultiplied_alpha};
    };

    class VStyleActor;
    using spVStyleActor = intrusive_ptr<VStyleActor>;
    class VStyleActor : public Actor
    {
    public:
        explicit VStyleActor();
        virtual ~VStyleActor() = default;

        VideoDriver::blend_mode getBlendMode() const
        {
#ifdef GRAPHICSUPPORT
            return m_vstyle.getBlendMode();
#else
            return VideoDriver::blend_mode::blend_disabled;
#endif
        }
        const QColor& getColor() const;
        const QColor& getAddColor() const;

        void setColor(const QColor& color);
        void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        void setBlendMode(VideoDriver::blend_mode mode);
        void setAddColor(const QColor& color);
        void setAddColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0);
        virtual void setEnabled(bool value) override;
        typedef Property<QColor, const QColor&, VStyleActor, &VStyleActor::getColor, &VStyleActor::setColor> TweenColor;
        typedef Property<QColor, const QColor&, VStyleActor, &VStyleActor::getAddColor, &VStyleActor::setAddColor> TweenAddColor;
        QColor getDisableColor() const;
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
        VisualStyle m_vstyle;
        QColor m_disableColor{75, 75, 75, 0};
        spMaterial m_mat;
#else
        static QColor m_dummyColor;
#endif

    };

}
