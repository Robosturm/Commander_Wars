#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"

namespace oxygine
{
    class ResFont;
    using spResFont = intrusive_ptr<ResFont>;
    class ResFont : public Resource
    {
    public:
        explicit ResFont() = default;
        virtual ~ResFont() = default;
        virtual const Font* getFont(const QString & name = "", qint32 size = 0) const = 0;
        qint32 getSize() const
        {
            return m_size;
        }
        virtual const Font* getClosestFont(float worldScale, qint32 styleFontSize, float& resScale) const;
    protected:
        qint32 m_size{0};
    };
}
