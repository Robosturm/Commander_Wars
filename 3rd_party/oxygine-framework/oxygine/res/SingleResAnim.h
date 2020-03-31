#pragma once
#include "../oxygine-include.h"
#include "ResAnim.h"

namespace oxygine
{
    class SingleResAnim: public ResAnim
    {
    public:
        void init(QString file, int columns = 1, int rows = 1, float scaleFactor = 1.0f);
        void init(Image* original, int columns = 1, int rows = 1, float scaleFactor = 1.0f) override;

        spNativeTexture getTexture() const;

    protected:
        QVector<unsigned char> _data;
        spNativeTexture _texture;
    };
}
