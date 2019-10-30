#include "tweenaddcolorall.h"

TweenAddColorAll::TweenAddColorAll(QColor addColor)
    : m_addColor(addColor)
{

}

void TweenAddColorAll::update(oxygine::VStyleActor& actor, float p, const oxygine::UpdateState&)
{
    QColor addColor(m_addColor.red() * p, m_addColor.green() * p, m_addColor.blue() * p, m_addColor.alpha() * p);
    actor.setAddColor(addColor);
    oxygine::spVStyleActor child = static_cast<oxygine::VStyleActor*>(actor.getFirstChild().get());
    while (child)
    {
        child->setAddColor(addColor);
        child = static_cast<oxygine::VStyleActor*>(child->getNextSibling().get());
    }
}
