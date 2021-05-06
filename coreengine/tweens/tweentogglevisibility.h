#ifndef TWEENTOGGLEVISIBILITY_H
#define TWEENTOGGLEVISIBILITY_H

#include "3rd_party/oxygine-framework/oxygine-include.h"

class TweenToggleVisibility
{
public:
    typedef oxygine::Actor type;
    /**
     * @brief TweenToggleVisibility. The sprite is visible between start and end
     * @param start fraction between 0 and 1
     * @param end fraction between 0 and 1
     */
    explicit TweenToggleVisibility(float start, float end);

    void update(oxygine::Actor& actor, float p, const oxygine::UpdateState& us);

    void init(oxygine::Actor&){}
    void done(oxygine::Actor&){}
protected:
    float startFraction{0};
    float endFracrtion{0};
};

#endif // TWEENTOGGLEVISIBILITY_H
