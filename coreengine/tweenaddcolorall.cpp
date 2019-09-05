#include "tweenaddcolorall.h"

TweenAddColorAll::TweenAddColorAll(oxygine::Color addColor)
    : m_addColor(addColor)
{

}

void TweenAddColorAll::update(oxygine::VStyleActor& actor, float p, const oxygine::UpdateState&)
{
    oxygine::Color addColor(m_addColor.r * p, m_addColor.g * p, m_addColor.b * p, m_addColor.a * p);
    actor.setAddColor(addColor);
    oxygine::spVStyleActor child = static_cast<oxygine::VStyleActor*>(actor.getFirstChild().get());
    while (child)
    {
        child->setAddColor(addColor);
        child = static_cast<oxygine::VStyleActor*>(child->getNextSibling().get());
    }
}
