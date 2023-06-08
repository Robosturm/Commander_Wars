#pragma once

#include "wiki/wikipage.h"

class DamageTablePage;
using spDamageTablePage = oxygine::intrusive_ptr<DamageTablePage>;

class DamageTablePage final : public Wikipage
{
public:
    explicit DamageTablePage(const QString & pageId);
   virtual ~DamageTablePage() = default;
private:
    oxygine::spColorRectSprite m_verticalMarker;
    oxygine::spColorRectSprite m_horizontalMarker;
};

