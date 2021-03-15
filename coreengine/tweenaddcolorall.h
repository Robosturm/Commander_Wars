#ifndef TWEENADDCOLORALL_H
#define TWEENADDCOLORALL_H


#include "3rd_party/oxygine-framework/oxygine-framework.h"


class TweenAddColorAll
{
public:
    typedef oxygine::VStyleActor type;
    TweenAddColorAll(QColor addColor, bool all = true);

    void update(oxygine::VStyleActor& actor, float p, const oxygine::UpdateState& us);

    void init(oxygine::VStyleActor& actor){}
    void done(oxygine::VStyleActor& actor){}
protected:
    QColor m_addColor;
    bool m_all{true};
};

#endif // TWEENADDCOLORALL_H
