#include "coreengine/tweens/tweenaddcolorall.h"

TweenAddColorAll::TweenAddColorAll(QColor addColor, bool all)
    : m_addColor(addColor),
      m_all(all)
{

}

void TweenAddColorAll::update(oxygine::VStyleActor& actor, float p, const oxygine::UpdateState&)
{
    QColor addColor(m_addColor.red() * p, m_addColor.green() * p, m_addColor.blue() * p, m_addColor.alpha() * p);
    actor.setAddColor(addColor);
    if (m_all)
    {
        oxygine::spVStyleActor child = static_cast<oxygine::VStyleActor*>(actor.getFirstChild().get());
        while (child)
        {
            child->setAddColor(addColor);
            child = static_cast<oxygine::VStyleActor*>(child->getNextSibling().get());
        }
    }
}
