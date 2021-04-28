#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
    class SingleResAnim : public QObject, public ResAnim
    {
        Q_OBJECT
    public:
        explicit SingleResAnim();
        void init(QString file, qint32 columns = 1, qint32 rows = 1, float scaleFactor = 1.0f);
        void init(Image* original, qint32 columns = 1, qint32 rows = 1, float scaleFactor = 1.0f) override;

        spNativeTexture getTexture() const;

    protected:
        QVector<unsigned char> _data;
        spNativeTexture _texture;
    };
}
