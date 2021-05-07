#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

namespace oxygine
{
    DECLARE_SMART(MaskedSprite, spMaskedSprite);
    class MaskedSprite: public Sprite
    {
    public:
        MaskedSprite();
        ~MaskedSprite();

        spSprite    getMask() const;
        bool        getUseRChannel() const;

        void setMask(spSprite, bool useRChannel = false);
    protected:
        void render(const RenderState& parentRS) override;

    private:
        spSprite m_mask;
        bool m_useRChannel;
    };
}
