#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

namespace oxygine
{
    class VisualStyle
    {
    public:

        VisualStyle();

        const QColor&        getColor() const {return _color;}
        blend_mode          getBlendMode() const {return _blend;}

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
        VStyleActor();

        blend_mode              getBlendMode() const {return _vstyle.getBlendMode();}
        const QColor&            getColor() const;
        const QColor&            getAddColor() const;

        void                    setColor(const QColor& color);
        void                    setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        void                    setBlendMode(blend_mode mode);
        void                    setAddColor(const QColor& color);
        void                    setAddColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0);

        virtual void setEnabled(bool value) override;

        typedef Property<QColor, const QColor&, VStyleActor, &VStyleActor::getColor, &VStyleActor::setColor> TweenColor;
        typedef Property<QColor, const QColor&, VStyleActor, &VStyleActor::getAddColor, &VStyleActor::setAddColor> TweenAddColor;

        void                    setMaterial(spSTDMaterial mat);
        void                    resetMaterial();
        spSTDMaterial _mat;

        QColor getDisableColor() const;
        void setDisableColor(const QColor &value);

    protected:
        virtual void matChanged() {}
        VisualStyle _vstyle;
        QColor disableColor{75, 75, 75, 0};

    };

}
