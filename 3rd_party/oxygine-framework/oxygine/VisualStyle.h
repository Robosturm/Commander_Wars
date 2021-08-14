#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

namespace oxygine
{
    class VisualStyle
    {
    public:
        explicit VisualStyle();
        virtual ~VisualStyle() = default;

        const QColor&       getColor() const
        {
            return _color;
        }
        blend_mode          getBlendMode() const
        {
            return _blend;
        }

        void setColor(const QColor& color);
        void setBlendMode(blend_mode mode);

    protected:
        QColor _color;
        blend_mode _blend;
    };

    DECLARE_SMART(VStyleActor, spVStyleActor);
    class VStyleActor : public Actor
    {
    public:
        explicit VStyleActor();
        virtual ~VStyleActor() = default;

        blend_mode              getBlendMode() const
        {
            return m_vstyle.getBlendMode();
        }
        const QColor&           getColor() const;
        const QColor&           getAddColor() const;

        void                    setColor(const QColor& color);
        void                    setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        void                    setBlendMode(blend_mode mode);
        void                    setAddColor(const QColor& color);
        void                    setAddColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0);

        virtual void setEnabled(bool value) override;

        typedef Property<QColor, const QColor&, VStyleActor, &VStyleActor::getColor, &VStyleActor::setColor> TweenColor;
        typedef Property<QColor, const QColor&, VStyleActor, &VStyleActor::getAddColor, &VStyleActor::setAddColor> TweenAddColor;
        QColor getDisableColor() const;
        void setDisableColor(const QColor &value);

        inline spMaterial & getMaterial()
        {
            return m_mat;
        }
    protected:
        virtual void matChanged() {}
        void setMaterial(spMaterial mat);
    private:
        void changeAddColor(const QColor& color);
    protected:
        VisualStyle m_vstyle;
        QColor m_disableColor{75, 75, 75, 0};
        spMaterial m_mat;

    };

}
