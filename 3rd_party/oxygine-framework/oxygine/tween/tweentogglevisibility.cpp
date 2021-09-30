#include "3rd_party/oxygine-framework/oxygine/tween/tweentogglevisibility.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

TweenToggleVisibility::TweenToggleVisibility(float start, float end)
    : m_startFraction(start),
      m_endFracrtion(end)
{
}

void TweenToggleVisibility::update(oxygine::Actor& actor, float p, const oxygine::UpdateState&)
{
    if ((p >= m_startFraction) && (p <= m_endFracrtion))
    {
        actor.setVisible(true);
    }
    else
    {
        actor.setVisible(false);
    }
}
