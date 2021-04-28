#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"

namespace oxygine
{
    /** A TweenAnim class
    *   use for playing per frame animation
    \code
    spSprite sprite = new Sprite();
    sprite->addTween(TweenAnim(res.getResAnim("anim")), 500, -1);
    \endcode
    */
    class TweenAnim
    {
    public:
        typedef Sprite type;

        /**Row/Column mode. Plays single row. if resanim == 0 will be used current*/
        TweenAnim(const ResAnim* resAnim, qint32 row = 0);

        TweenAnim(const ResAnim* resAnim, float initFrame, qint32 row);

        /**Frames mode. Play animation in interval [start, end]*/
        TweenAnim(const ResAnim* resAnim, qint32 startFrame, qint32 endFrame);

        void init(Sprite& actor);
        void done(Sprite&) {}

        const ResAnim*  getResAnim() const {return _resAnim;}
        qint32             getRow() const {return _row;}
        qint32             getStart() const {return _start;}
        qint32             getEnd() const {return _end;}

        /**Changes ResAnim*/
        void setResAnim(const ResAnim* resAnim);

        /**Play animation in interval [start, end]*/
        void setInterval(qint32 start, qint32 end);

        void update(Sprite& actor, float p, const UpdateState& us);

    protected:
        virtual void _setAnimFrame(Sprite&, const AnimationFrame&);
        const ResAnim* _resAnim;
        qint32 _row;
        qint32 _start;
        qint32 _end;
        float _initFrame{0.0f};
    };
}
