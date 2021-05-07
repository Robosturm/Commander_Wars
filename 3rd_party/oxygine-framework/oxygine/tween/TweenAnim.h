#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"

namespace oxygine
{
    /** A TweenAnim class
    *   use for playing per frame animation
    \code
    spSprite sprite = spSprite::create();
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

        const ResAnim*  getResAnim() const {return m_resAnim;}
        qint32             getRow() const {return m_row;}
        qint32             getStart() const {return m_start;}
        qint32             getEnd() const {return m_end;}

        /**Changes ResAnim*/
        void setResAnim(const ResAnim* resAnim);

        /**Play animation in interval [start, end]*/
        void setInterval(qint32 start, qint32 end);

        void update(Sprite& actor, float p, const UpdateState& us);

    protected:
        virtual void _setAnimFrame(Sprite&, const AnimationFrame&);

    protected:
        const ResAnim* m_resAnim;
        qint32 m_row;
        qint32 m_start;
        qint32 m_end;
        float m_initFrame{0.0f};
    };
}
