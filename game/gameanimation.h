#ifndef GAMEANIMATION_H
#define GAMEANIMATION_H

#include <QObject>

#include <QVector>

#include <QColor>

#include "oxygine-framework.h"

#include "game/smartpointers.h"

class GameAnimation : public QObject, public oxygine::Sprite
{
    Q_OBJECT
public:
    explicit GameAnimation(quint32 frameTime);
    virtual ~GameAnimation() = default;

    virtual void restart();
    virtual void stop(){};
signals:
    void sigFinished();
public slots:
    /**
     * @brief setRotation sets the rotation of this animation
     * @param angle angle in degree 0° to 360°
     */
    void setRotation(float angle);
    /**
     * @brief queueAnimation
     * @param pGameAnimation
     */
    void queueAnimation(GameAnimation* pGameAnimation);
    /**
     * @brief addSprite
     * @param spriteID
     */
    void addSprite(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish = 0, float scale = 1.0f, qint32 delay = 0);
    /**
     * @brief addSprite
     * @param spriteID
     */
    void addSprite2(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish = 0, float scaleX = 1.0f, float scaleY = 1.0f, qint32 delay = 0);
    /**
     * @brief addText
     * @param text
     */
    void addText(QString text, float offsetX, float offsetY, float scale, QColor color = Qt::GlobalColor::black);
    /**
     * @brief onFinished called when the animation is finished
     */
    virtual bool onFinished();
    /**
     * @brief setEndOfAnimationCall calls a java script function when the animation is finished. Note: no parameters can be used to call the function
     * @param postActionObject java script object that will be used
     * @param postActionFunction java script function of the object that will be called
     */
    void setEndOfAnimationCall(QString postActionObject, QString postActionFunction);
    /**
     * @brief setSound sets the sound to be played during this animation
     * @param soundFile sound file
     * @param loops amount of loops to play this sound -1 till the animation is over;
     */
    void setSound(QString soundFile, qint32 loops);
    /**
     * @brief addTweenScale adds a tween to change the scale of this animation
     * @param endScale final scaling of the animation
     * @param duration duration of scaling up
     */
    void addTweenScale(float endScale, qint32 duration);
    /**
     * @brief addTweenPosition adds a tween to change the position of this animation
     * @param point final position of the animation
     * @param duration duration of scaling up
     */
    void addTweenPosition(QPoint point, qint32 duration);
    /**
     * @brief addTweenColor
     * @param startColor
     * @param endColor
     * @param duration
     * @param twoSided
     */
    void addTweenColor(qint32 spriteIdx, QColor startColor, QColor endColor, qint32 duration, bool twoSided, qint32 delay = 0);
protected:
    virtual void update(const oxygine::UpdateState& us) override;
private:
    quint32 m_frameTime{0};
    QVector<GameAnimation*> m_QueuedAnimations;
    QString jsPostActionObject{""};
    QString jsPostActionFunction{""};

    QString m_soundFile;
    qint32 m_loops;
    bool m_SoundStarted{false};
};

#endif // GAMEANIMATION_H
