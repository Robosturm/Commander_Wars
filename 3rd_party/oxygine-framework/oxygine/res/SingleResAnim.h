#pragma once
#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
    class SingleResAnim;
using spSingleResAnim = std::shared_ptr<SingleResAnim>;
    class SingleResAnim final : public QObject, public ResAnim
    {
        Q_OBJECT
    public:
        explicit SingleResAnim();
        virtual ~SingleResAnim();
        void init(const QString & file, qint32 columns, qint32 rows, float scaleFactor) override;
        void init(QImage & image, qint32 columns, qint32 rows, float scaleFactor,
                          bool clamp2Edge = true, quint32 linearFilter = 0, bool convertImg = true) override;
        spTexture getTexture() const;

    protected:
        spTexture m_texture;
    };
}
