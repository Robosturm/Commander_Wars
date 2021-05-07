#include "3rd_party/oxygine-framework/oxygine/tween/TweenAnim.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
    TweenAnim::TweenAnim(const ResAnim* resAnim, qint32 row):
        m_resAnim(resAnim),
        m_row(row),
        m_start(0),
        m_end(0)
    {
        if (m_resAnim)
        {
            m_end = m_resAnim->getColumns() - 1;
        }
    }

    TweenAnim::TweenAnim(const ResAnim* resAnim, float initFrame, qint32 row)
        :m_resAnim(resAnim),
          m_row(row),
          m_start(0),
          m_end(0),
         m_initFrame(initFrame)
      {
          if (m_resAnim)
          {
              m_end = m_resAnim->getColumns() - 1;
          }
      }

    TweenAnim::TweenAnim(const ResAnim* resAnim, qint32 startFrame, qint32 endFrame):
        m_resAnim(resAnim),
        m_row(-1),
        m_start(0),
        m_end(0)
    {
        setInterval(startFrame, endFrame);
    }

    void TweenAnim::init(Sprite& actor)
    {
        if (!m_resAnim)
        {
            m_resAnim = actor.getResAnim();
            m_end = m_resAnim->getColumns() - 1;
        }
    }


    void TweenAnim::setResAnim(const ResAnim* resAnim)
    {
        m_resAnim = resAnim;
        if (m_resAnim && m_row != -1)
        {
            m_end = m_resAnim->getColumns() - 1;
        }
    }

    void TweenAnim::setInterval(qint32 start, qint32 end)
    {
        m_start = start;
        m_end = end;

        qint32 num = m_row == -1 ? m_resAnim->getTotalFrames() : m_resAnim->getColumns();
        Q_ASSERT(m_start >= 0 && m_start < num);
        Q_ASSERT(m_end >= 0 && m_end < num);
    }

    void TweenAnim::_setAnimFrame(Sprite& actor, const AnimationFrame& frame)
    {
        actor.setAnimFrame(frame);
    }

    void TweenAnim::update(Sprite& actor, float p, const UpdateState&)
    {
        Q_ASSERT(m_resAnim);
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

        const AnimationFrame& fr = m_row == -1 ? m_resAnim->getFrame(frame) : m_resAnim->getFrame(frame, m_row);
        _setAnimFrame(actor, fr);
    }
}
