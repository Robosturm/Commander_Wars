#pragma once
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"
#include "3rd_party/oxygine-framework/oxygine/actor/VisualStyleActor.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/tween/TweenAnim.h"

namespace oxygine
{
class Sprite;
using spSprite = std::shared_ptr<Sprite>;
class Sprite : public VStyleActor
{
public:
    static constexpr const char* const getTypeName()
    {
        return "Sprite";
    }
    using TweenAnim = oxygine::TweenAnim;
    explicit Sprite() = default;
    virtual ~Sprite();

#ifdef GRAPHICSUPPORT
    const AnimationFrame& getAnimFrame() const
    {
        return m_frame;
    }
#endif
    QRect getDestRect() const override;

    bool getManageResAnim() const
    {
#ifdef GRAPHICSUPPORT
        return (m_flags & flag_manageResAnim) != 0;
#else
        return true;
#endif
    }
    const QRectF& getSrcRect() const
    {
#ifdef GRAPHICSUPPORT
        return m_frame.getSrcRect();
#else
        return m_dummyRectF;
#endif
    }
    const ResAnim* getResAnim() const
    {
#ifdef GRAPHICSUPPORT
        return m_frame.getResAnim();
#else
        return nullptr;
#endif
    }
    qint32 getColumn() const
    {
#ifdef GRAPHICSUPPORT
        return m_frame.getColumn();
#else
        return 0;
#endif
    }
    qint32 getRow() const
    {
#ifdef GRAPHICSUPPORT
        return m_frame.getRow();
#else
        return 0;
#endif
    }

    /**load/unload atlas automatically or not*/
    void setManageResAnim(bool manage);
    /**Changes sprite image*/
    void setAnimFrame(const AnimationFrame& f);
    /**Takes AnimationFrame from ResAnim and set it as current to Sprite. Shows assert is resanim is null. Using this method is more safe than 'setAnimFrame(const AnimationFrame &f)'*/
    void setAnimFrame(const ResAnim* resanim, qint32 col = 0, qint32 row = 0);
    void setColorTable(const oxygine::spResAnim pAnim, bool matrix);
    virtual void setResAnim(const ResAnim* resanim, qint32 col = 0, qint32 row = 0);
    void setRow(qint32 row);
    void setColumn(qint32 column);
    void setColumnRow(qint32 column, qint32 row);
    virtual bool isOn(const QPoint& localPosition) override;
    bool isFlippedX() const
    {
#ifdef GRAPHICSUPPORT
        return (m_flags & flag_flipX) != 0;
#else
        return false;
#endif
    }
    bool isFlippedY() const
    {
#ifdef GRAPHICSUPPORT
        return (m_flags & flag_flipY) != 0;
#else
        return false;
#endif
    }
    void setFlippedX(bool flippedX);
    virtual void flipActorsX(bool flippedX)
    {
        flipActorsX(getSharedPtr<Actor>(), flippedX);
    }
    static void flipActorsX(oxygine::spActor pActor, bool flippedX);
    void setFlippedY(bool flippedY);
    void setFlipped(bool flippedX, bool flippedY);
    bool getInvertFlipX() const;
    void setInvertFlipX(bool value);
    virtual void doRender(const RenderState&) override;


protected:
    friend MemoryManagement;
#ifdef GRAPHICSUPPORT
    enum
        {
            flag_manageResAnim = flag_last << 1,
            flag_flipX = flag_last << 2,
            flag_flipY = flag_last << 3
        };
#endif
    virtual void changeAnimFrame(const AnimationFrame& f);
    virtual void animFrameChanged(const AnimationFrame& f);

protected:
#ifdef GRAPHICSUPPORT
    AnimationFrame m_frame;
    oxygine::spResAnim m_colorTable;
    bool m_invertFlipX{false};
#else
    static QRectF m_dummyRectF;
#endif
};
}
