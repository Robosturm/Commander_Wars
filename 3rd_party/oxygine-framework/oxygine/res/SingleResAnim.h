#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
    class SingleResAnim;
    using spSingleResAnim = intrusive_ptr<SingleResAnim>;
    class SingleResAnim : public QObject, public ResAnim
    {
        Q_OBJECT
    public:
        explicit SingleResAnim();
        virtual ~SingleResAnim();
        void init(QString file, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder);
        virtual void init(Image* original, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder) override;
        spTexture getTexture() const;

    protected:
        QVector<unsigned char> m_data;
        spTexture m_texture;
    };
}
