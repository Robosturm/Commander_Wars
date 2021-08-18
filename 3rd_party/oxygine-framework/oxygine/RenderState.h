#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/math/AffineTransform.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

namespace oxygine
{

    class RenderState
    {
    public:
        explicit RenderState()
      {
          transform.identity();
      }

      QColor getFinalColor(const QColor& clr) const
      {
          QColor color = clr;
          color.setAlpha((color.alpha() * static_cast<qint32>(alpha)) / 255);
          return color;
      }

      AffineTransform transform;
      unsigned char alpha{255};
      const RectF* clip{nullptr};
    };
}
