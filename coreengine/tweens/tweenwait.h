#ifndef TWEENWAIT_H
#define TWEENWAIT_H

#include "3rd_party/oxygine-framework/oxygine-include.h"

class TweenWait
{
public:
    typedef oxygine::Actor type;

    explicit TweenWait();

    void update(oxygine::Actor& actor, float p, const oxygine::UpdateState& us);

    void init(oxygine::Actor&){}
    void done(oxygine::Actor&){}
protected:
};

#endif // TWEENWAIT_H
