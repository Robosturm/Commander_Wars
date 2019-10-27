#pragma once
#include "../oxygine-include.h"
#include "Sprite.h"

namespace oxygine
{
    DECLARE_SMART(MaskedSprite, spMaskedSprite);
    class MaskedSprite: public Sprite
    {
    public:
        DECLARE_COPYCLONE_NEW(MaskedSprite);

        MaskedSprite();
        ~MaskedSprite();

        spSprite    getMask() const;
        bool        getUseRChannel() const;

        void setMask(spSprite, bool useRChannel = false);
    protected:
        void render(const RenderState& parentRS) override;

    private:
        spSprite _mask;
        bool _useRChannel;
    };
}
