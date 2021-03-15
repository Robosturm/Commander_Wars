#include "3rd_party/oxygine-framework/oxygine/tween/TweenAnim.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
    TweenAnim::TweenAnim(const ResAnim* resAnim, int row):
        _resAnim(resAnim),
        _row(row),
        _start(0),
        _end(0)
    {
        if (_resAnim)
        {
            _end = _resAnim->getColumns() - 1;
        }
    }

    TweenAnim::TweenAnim(const ResAnim* resAnim, float initFrame, int row)
        :_resAnim(resAnim),
          _row(row),
          _start(0),
          _end(0),
         _initFrame(initFrame)
      {
          if (_resAnim)
          {
              _end = _resAnim->getColumns() - 1;
          }
      }

    TweenAnim::TweenAnim(const ResAnim* resAnim, int startFrame, int endFrame):
        _resAnim(resAnim),
        _row(-1),
        _start(0),
        _end(0)
    {
        setInterval(startFrame, endFrame);
    }

    void TweenAnim::init(Sprite& actor)
    {
        if (!_resAnim)
        {
            _resAnim = actor.getResAnim();
            _end = _resAnim->getColumns() - 1;
        }
    }


    void TweenAnim::setResAnim(const ResAnim* resAnim)
    {
        _resAnim = resAnim;
        if (_resAnim && _row != -1)
            _end = _resAnim->getColumns() - 1;
    }

    void TweenAnim::setInterval(int start, int end)
    {
        _start = start;
        _end = end;

        int num = _row == -1 ? _resAnim->getTotalFrames() : _resAnim->getColumns();
        Q_ASSERT(_start >= 0 && _start < num);
        Q_ASSERT(_end >= 0 && _end < num);
    }

    void TweenAnim::_setAnimFrame(Sprite& actor, const AnimationFrame& frame)
    {
        actor.setAnimFrame(frame);
    }

    void TweenAnim::update(Sprite& actor, float p, const UpdateState&)
    {
        Q_ASSERT(_resAnim);
        int frame;

        p += _initFrame;
        if (p > 1.0f)
        {
            p -= 1.0f;
        }

        if (p >= 1.0f)
        {
            p = 0.9999999f;
        }

        if (_start > _end)
        {
            frame = - int((_start + 1 - _end) * p);
        }
        else
        {
            frame =   int((_end   + 1 - _start) * p);
        }

        frame += _start;

        const AnimationFrame& fr = _row == -1 ? _resAnim->getFrame(frame) : _resAnim->getFrame(frame, _row);
        _setAnimFrame(actor, fr);
    }
}
