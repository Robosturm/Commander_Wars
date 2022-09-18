#pragma once

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

class TweenChangeNumberText final
{
public:
    using TActor = oxygine::TextField;

    explicit TweenChangeNumberText(qint32 startNumber, qint32 endNumber);
    ~TweenChangeNumberText() = default;
    void update(oxygine::TextField & actor, float p, const oxygine::UpdateState& us);

    void init(oxygine::TextField&);
    void done(oxygine::TextField&);
protected:
    qint32 m_startNumber{0};
    qint32 m_endNumber{0};
};
