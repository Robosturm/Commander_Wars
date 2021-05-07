#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"

namespace oxygine
{
    DECLARE_SMART(ResFont, spResFont);
    class ResFont : public Resource
    {
    public:
        ResFont()
            : m_size(0)
        {
        }

        virtual const Font* getFont(QString name = "", qint32 size = 0) const = 0;
        qint32 getSize() const { return m_size; }

        virtual const Font* getClosestFont(float worldScale, qint32 styleFontSize, float& resScale) const;
    protected:
        qint32 m_size;
    };
}
