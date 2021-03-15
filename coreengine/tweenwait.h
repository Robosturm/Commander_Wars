#ifndef TWEENWAIT_H
#define TWEENWAIT_H

#include "3rd_party/oxygine-framework/oxygine-include.h"

class TweenWait
{
public:
    typedef oxygine::Sprite type;

    explicit TweenWait();

    void update(oxygine::Sprite& actor, float p, const oxygine::UpdateState& us);

    void init(oxygine::Sprite& actor){}
    void done(oxygine::Sprite& actor){}
protected:
};

#endif // TWEENWAIT_H
