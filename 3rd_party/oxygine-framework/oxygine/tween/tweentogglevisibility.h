#ifndef TWEENTOGGLEVISIBILITY_H
#define TWEENTOGGLEVISIBILITY_H

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

class TweenToggleVisibility final
{
public:
    using TActor = oxygine::Actor;
    /**
     * @brief TweenToggleVisibility. The sprite is visible between start and end
     * @param start fraction between 0 and 1
     * @param end fraction between 0 and 1
     */
    explicit TweenToggleVisibility(float start, float end);
   virtual ~TweenToggleVisibility() = default;
    void update(oxygine::Actor& actor, float p, const oxygine::UpdateState& us);

    void init(oxygine::Actor&){}
    void done(oxygine::Actor&){}
protected:
    float m_startFraction{0};
    float m_endFracrtion{0};
};

#endif // TWEENTOGGLEVISIBILITY_H
