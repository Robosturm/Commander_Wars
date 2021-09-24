#include "3rd_party/oxygine-framework/oxygine/tween/TweenAnimColumn.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
    TweenAnimColumn::TweenAnimColumn(const ResAnim* resAnim, qint32 column):
        m_resAnim(resAnim),
        m_column(column)
    {
        if (m_resAnim)
        {
            m_end = m_resAnim->getRows() - 1;
        }
    }

    TweenAnimColumn::TweenAnimColumn(const ResAnim* resAnim, float initFrame, qint32 column)
        :m_resAnim(resAnim),
          m_column(column),
          m_start(0),
          m_end(0),
         m_initFrame(initFrame)
      {
          if (m_resAnim)
          {
              m_end = m_resAnim->getRows() - 1;
          }
      }

    TweenAnimColumn::TweenAnimColumn(const ResAnim* resAnim, qint32 startFrame, qint32 endFrame):
        m_resAnim(resAnim),
        m_column(-1),
        m_start(0),
        m_end(0)
    {
        setInterval(startFrame, endFrame);
    }

    void TweenAnimColumn::init(Sprite& actor)
    {
        if (!m_resAnim)
        {
            m_resAnim = actor.getResAnim();
            m_end = m_resAnim->getRows() - 1;
        }
    }


    void TweenAnimColumn::setResAnim(const ResAnim* resAnim)
    {
        m_resAnim = resAnim;
        if (m_resAnim && m_column != -1)
        {
            m_end = m_resAnim->getRows() - 1;
        }
    }

    void TweenAnimColumn::setInterval(qint32 start, qint32 end)
    {
        m_start = start;
        m_end = end;

        qint32 num = m_column == -1 ? m_resAnim->getTotalFrames() : m_resAnim->getRows();
        if (m_start < 0 || m_start >= num)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "TweenAnim::setInterval invalid start");
        }
        if (m_end < 0 || m_end >= num)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "TweenAnim::setInterval invalid end");
        }
    }

    void TweenAnimColumn::_setAnimFrame(Sprite& actor, const AnimationFrame& frame)
    {
        actor.setAnimFrame(frame);
    }

    void TweenAnimColumn::update(Sprite& actor, float p, const UpdateState&)
    {
        if (m_resAnim == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "TweenAnim::update no resAnim specified");
            return;
        }
        qint32 frame;

        p += m_initFrame;
        if (p > 1.0f)
        {
            p -= 1.0f;
        }

        if (p >= 1.0f)
        {
            p = 0.9999999f;
        }

        if (m_start > m_end)
        {
            frame = - int((m_start + 1 - m_end) * p);
        }
        else
        {
            frame =   int((m_end   + 1 - m_start) * p);
        }

        frame += m_start;

        const AnimationFrame& fr = m_column == -1 ? m_resAnim->getFrame(frame) : m_resAnim->getFrame(m_column, frame);
        _setAnimFrame(actor, fr);
    }
}
