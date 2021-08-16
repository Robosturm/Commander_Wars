#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

namespace oxygine
{
    class MaskedSprite;
    using spMaskedSprite = intrusive_ptr<MaskedSprite>;
    class MaskedSprite: public Sprite
    {
    public:
        explicit MaskedSprite() = default;
        virtual ~MaskedSprite() = default;
        Sprite* getMask() const;
        bool getUseRChannel() const;
        void setMask(spSprite, bool useRChannel = false);
    protected:
        virtual void render(const RenderState& parentRS) override;

    private:
        spSprite m_mask;
        bool m_useRChannel{false};
    };
}
