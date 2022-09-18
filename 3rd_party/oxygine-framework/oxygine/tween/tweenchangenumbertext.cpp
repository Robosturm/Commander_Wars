#include "3rd_party/oxygine-framework/oxygine/tween/tweenchangenumbertext.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"

TweenChangeNumberText::TweenChangeNumberText(qint32 startNumber, qint32 endNumber)
    : m_startNumber(startNumber),
      m_endNumber(endNumber)
{

}

void TweenChangeNumberText::update(oxygine::TextField & actor, float p, const oxygine::UpdateState& us)
{
    qint32 number = static_cast<qint32>((1.0f - p) * (m_startNumber - m_endNumber) + m_endNumber);
    actor.setText(QString::number(number), false);
}

void TweenChangeNumberText::init(oxygine::TextField&)
{
}

void TweenChangeNumberText::done(oxygine::TextField&)
{
}
