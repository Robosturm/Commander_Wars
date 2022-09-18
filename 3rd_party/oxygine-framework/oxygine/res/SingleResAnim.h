#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
    class SingleResAnim;
    using spSingleResAnim = intrusive_ptr<SingleResAnim>;
    class SingleResAnim final : public ResAnim
    {
    public:
        explicit SingleResAnim();
        ~SingleResAnim();
        virtual void init(const QString & file, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder) override;
        virtual void init(QImage & image, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder,
                          bool clamp2Edge = true, quint32 linearFilter = 0) override;
        spTexture getTexture() const;

    protected:
        spTexture m_texture;
    };
}
