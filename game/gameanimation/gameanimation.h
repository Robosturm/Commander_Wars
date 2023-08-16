#ifndef GAMEANIMATION_H
#define GAMEANIMATION_H

#include <QObject>
#include <QVector>
#include <QColor>
#include <QBuffer>
#include <QDataStream>

#include "game/GameEnums.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/tween/TweenQueue.h"
#include "3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h"

class Player;
class GameMap;
class GameAnimation;
using spGameAnimation = std::shared_ptr<GameAnimation>;

class GameAnimation : public QObject, public oxygine::Sprite
{
    Q_OBJECT
public:
    explicit GameAnimation(quint32 frameTime, GameMap* pMap);
    virtual ~GameAnimation() = default;
    virtual void restart();
    virtual void stop();


signals:
    void sigFinished(bool skipping);
    void sigStart();
public:
    /**
     * @brief getFontWidth
     * @param font
     * @param text
     * @return
     */
    Q_INVOKABLE qint32 getFontWidth(const QString font, const QString text) const;
    /**
     * @brief getMap
     * @return
     */
    Q_INVOKABLE GameMap *getMap() const;
    /**
     * @brief getStopSoundAtAnimationEnd
     * @return
     */
    Q_INVOKABLE bool getStopSoundAtAnimationEnd() const;
    /**
     * @brief setStopSoundAtAnimationEnd
     * @param stopSoundAtAnimationEnd
     */
    Q_INVOKABLE void setStopSoundAtAnimationEnd(bool stopSoundAtAnimationEnd);
    /**
     * @brief setRotation sets the rotation of this animation
     * @param angle angle in degree 0° to 360°
     */
    Q_INVOKABLE void setRotation(float angle);
    /**
     * @brief queueAnimation
     * @param pGameAnimation
     */
    Q_INVOKABLE void queueAnimation(GameAnimation* pGameAnimation);
    /**
     * @brief queueBefore
     * @param pGameAnimation
     */
    Q_INVOKABLE void queueAnimationBefore(GameAnimation* pGameAnimation);
    /**
     * @brief addSprite
     * @param spriteID
     * @param offsetX
     * @param offsetY
     * @param sleepAfterFinish
     * @param scale
     * @param delay
     * @param loops
     */
    Q_INVOKABLE void addSprite(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish = 0, float scale = 1.0f, qint32 delay = 0, qint32 loops = 1);
    /**
     * @brief addSprite2
     * @param spriteID
     * @param offsetX
     * @param offsetY
     * @param sleepAfterFinish
     * @param scaleX
     * @param scaleY
     * @param delay
     * @param loops
     */
    Q_INVOKABLE void addSprite2(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish = 0, float scaleX = 1.0f, float scaleY = 1.0f, qint32 delay = 0, qint32 loops = 1);
    /**
      * @brief addSprite3
      * @param spriteID
      * @param offsetX
      * @param offsetY
      * @param sleepAfterFinish
      * @param scaleX
      * @param scaleY
      * @param delay
      */
    Q_INVOKABLE void addSprite3(QString spriteID, float offsetX, float offsetY, QColor color, qint32 sleepAfterFinish = 0, float scaleX = 1.0f, float scaleY = 1.0f, qint32 delay = 0, qint32 frames = 0, qint32 loops = 1, QEasingCurve::Type easeType = QEasingCurve::Linear);
    /**
     * @brief addSpriteAnimTable
     * @param spriteID
     * @param offsetX
     * @param offsetY
     * @param pPlayer
     * @param sleepAfterFinish
     * @param scaleX
     * @param scaleY
     * @param delay
     * @param frames
     */
    Q_INVOKABLE void addSpriteAnimTable(QString spriteID, float offsetX, float offsetY, Player* pPlayer, qint32 sleepAfterFinish = 0, float scaleX = 1.0f, float scaleY = 1.0f, qint32 delay = 0, qint32 frames = 0, GameEnums::Recoloring mode = GameEnums::Recoloring_Table);
    /**
     * @brief addBox
     * @param spriteID
     * @param offsetX
     * @param offsetY
     * @param width
     * @param heigth
     * @param color
     * @param sleepAfterFinish
     * @param delay
     */
    Q_INVOKABLE void addBox(QString spriteID, float offsetX, float offsetY, qint32 width, qint32 heigth, qint32 sleepAfterFinish = 0, QColor color = Qt::white);
    /**
     * @brief addText
     * @param text
     * @return the width of the text
     */
    Q_INVOKABLE qint32 addText(QString text, float offsetX, float offsetY, float scale, QColor color = Qt::black, qint32 priority = 1);
    /**
     * @brief onFinished called when the animation is finished
     */
    Q_INVOKABLE virtual bool onFinished(bool skipping);
    /**
     * @brief setEndOfAnimationCall calls a java script function when the animation is finished. Note: no parameters can be used to call the function
     * @param postActionObject java script object that will be used
     * @param postActionFunction java script function of the object that will be called
     */
    Q_INVOKABLE void setEndOfAnimationCall(QString postActionObject, QString postActionFunction);
    /**
     * @brief setStartOfAnimationCall calls a java script function when the animation is started. Note: no parameters can be used to call the function
     * @param postActionObject java script object that will be used
     * @param postActionFunction java script function of the object that will be called
     */
    Q_INVOKABLE void setStartOfAnimationCall(QString preActionObject, QString preActionFunction);
    /**
     * @brief setSound sets the sound to be played during this animation
     * @param soundFile sound file
     * @param loops amount of loops to play this sound -1 till the animation is over;
     */
    Q_INVOKABLE void setSound(QString soundFile, qint32 loops = 1, qint32 delayMs = 0, float volume = 1.0f, bool stopOldestSound = false);
    /**
     * @brief addSound
     * @param soundFile
     * @param loops
     * @param folder
     * @param volume
     */
    Q_INVOKABLE void addSound(QString soundFile, qint32 loops = 1, qint32 delayMs = 0, float volume = 1.0f, bool stopOldestSound = false);
    /**
     * @brief addTweenScale adds a tween to change the scale of this animation
     * @param endScale final scaling of the animation
     * @param duration duration of scaling up
     */
    Q_INVOKABLE void addTweenScale(float endScale, qint32 duration, QEasingCurve::Type easeType = QEasingCurve::Linear);
    /**
     * @brief addTweenPosition adds a tween to change the position of this animation
     * @param point final position of the animation
     * @param duration duration of scaling up
     */
    Q_INVOKABLE void addTweenPosition(QPoint point, qint32 duration, QEasingCurve::Type easeType = QEasingCurve::Linear);
    /**
     * @brief addTweenColor
     * @param endColor
     * @param duration
     * @param twoSided
     */
    Q_INVOKABLE void addTweenColor(qint32 spriteIdx, QColor startColor, QColor endColor, qint32 duration, bool twoSided, qint32 delay = 0, QEasingCurve::Type easeType = QEasingCurve::Linear);
    /**
     * @brief addTweenWait
     * @param duration
     */
    Q_INVOKABLE void addTweenWait(qint32 duration, QEasingCurve::Type easeType= QEasingCurve::Linear);
    /**
     * @brief addTweenScale adds a tween to change the scale of this animation
     * @param endScale final scaling of the animation
     * @param duration duration of scaling up
     */
    Q_INVOKABLE void addTweenScaleToLastQueue(float endScale, qint32 duration, QEasingCurve::Type easeType = QEasingCurve::Linear);
    /**
     * @brief addTweenPosition adds a tween to change the position of this animation
     * @param point final position of the animation
     * @param duration duration of scaling up
     */
    Q_INVOKABLE void addTweenPositionToLastQueue(QPoint point, qint32 duration, QEasingCurve::Type easeType = QEasingCurve::Linear);
    /**
     * @brief addTweenColorToLastQueue
     * @param startColor
     * @param endColor
     * @param duration
     * @param twoSided
     */
    Q_INVOKABLE void addTweenColorToLastQueue(QColor endColor, qint32 duration, bool twoSided, qint32 delay = 0, QEasingCurve::Type easeType = QEasingCurve::Linear);
    /**
     * @brief addTweenWait
     * @param duration
     */
    Q_INVOKABLE void addTweenWaitToLastQueue(qint32 duration, QEasingCurve::Type easeType= QEasingCurve::Linear);

    /************ post animation stuff **************************/
    /**
     * @brief seekBuffer seeks the variable buffer to 0
     */
    Q_INVOKABLE void seekBuffer()
    {
        m_buffer.seek(0);
    }
    /**
     * @brief writeDataString adds a string to the action data
     * @param data
     */
    Q_INVOKABLE void writeDataString(QString data)
    {
        m_actionData << data;
    }
    /**
     * @brief readDataString
     * @return reads a string from the action data
     */
    Q_INVOKABLE QString readDataString()
    {
        QString data;
        if (m_buffer.size() > 0)
        {
            m_actionData >> data;
        }
        return data;
    }
    /**
     * @brief writeDataInt32 adds a int32 to the action data
     * @param data
     */
    Q_INVOKABLE void writeDataInt32(qint32 data)
    {
        m_actionData << data;
    }
    /**
     * @brief readDataInt32
     * @return reads a int32 from the action data
     */
    Q_INVOKABLE qint32 readDataInt32()
    {
        qint32 data = 0;
        if (m_buffer.size() > 0)
        {
            m_actionData >> data;
        }
        return data;
    }
    /**
     * @brief writeDataFloat adds a float to the action data
     * @param data
     */
    Q_INVOKABLE void writeDataFloat(float data)
    {
        m_actionData << data;
    }
    /**
     * @brief readDataFloat
     * @return reads a float from the action data
     */
    Q_INVOKABLE float readDataFloat()
    {
        float data = 0.0f;
        if (m_buffer.size() > 0)
        {
            m_actionData >> data;
        }
        return data;
    }
    /**
     * @brief writeDataFloat adds a float to the action data
     * @param data
     */
    Q_INVOKABLE void writeDataBool(bool data)
    {
        m_actionData << data;
    }
    /**
     * @brief readDataBool
     * @return reads a float from the action data
     */
    Q_INVOKABLE bool readDataBool()
    {
        bool data = false;
        if (m_buffer.size() > 0)
        {
            m_actionData >> data;
        }
        return data;
    }
    /**
     * @brief getVisible
     * @return
     */
    Q_INVOKABLE virtual bool getVisible() const override;
    /**
     * @brief setVisible
     * @param vis
     */
    Q_INVOKABLE virtual void setVisible(bool vis) override;
    /**
     * @brief addScreenshake
     * @param startIntensity
     * @param decay
     * @param durationMs
     * @param shakePauseMs
     */
    Q_INVOKABLE void addScreenshake(qint32 startIntensity, float decay, qint32 durationMs, qint32 delay = 0, qint32 shakePauseMs = 30, QEasingCurve::Type easeType = QEasingCurve::Linear);
    /**
     * @brief getFrameTime
     * @return
     */
    Q_INVOKABLE quint32 getFrameTime() const;
protected:
    void emitFinished();

private:
    /**
     * @brief loadSpriteAnim
     * @param pAnim
     * @param offsetX
     * @param offsetY
     * @param color
     * @param sleepAfterFinish
     * @param scaleX
     * @param scaleY
     * @param delay
     */
    void loadSpriteAnim(oxygine::ResAnim* pAnim, float offsetX, float offsetY, QColor color, qint32 sleepAfterFinish, float scaleX, float scaleY, qint32 delay, qint32 loops, QEasingCurve::Type easeType = QEasingCurve::Linear);
    /**
     * @brief loadSpriteAnimTable
     * @param pAnim
     * @param offsetX
     * @param offsetY
     * @param pPlayer
     * @param sleepAfterFinish
     * @param scaleX
     * @param scaleY
     * @param delay
     */
    void loadSpriteAnimTable(oxygine::ResAnim* pAnim, float offsetX, float offsetY, Player* pPlayer, qint32 sleepAfterFinish, float scaleX, float scaleY, qint32 delay, GameEnums::Recoloring mode);
    /**
     * @brief setPreviousAnimation
     * @param previousAnimation
     */
    void setPreviousAnimation(GameAnimation *previousAnimation);
    /**
     * @brief removeQueuedAnimation
     * @param pGameAnimation
     */
    void removeQueuedAnimation(GameAnimation* pGameAnimation);
protected slots:
    virtual void start();
protected:
    virtual void update(const oxygine::UpdateState& us) override;
    void doPreAnimationCall();
protected:
    quint32 m_frameTime{1};
    bool m_stopped{false};
    bool m_finishQueued{false};
    bool m_started{false};
    bool m_skipping{false};
    spGameAnimation m_previousAnimation{nullptr};
    struct SoundData
    {
        QString soundFile;
        qint32 loops;
        float volume{1.0f};
        qint32 delayMs;
        bool stopOldestSound{false};
    };
    QVector<SoundData> m_SoundData;
    GameMap* m_pMap{nullptr};
    oxygine::spTweenQueue m_lastCreatedTweenQueue;
private:

    QVector<spGameAnimation> m_QueuedAnimations;
    QString m_jsPostActionObject{""};
    QString m_jsPostActionFunction{""};
    QString m_jsPreActionObject{""};
    QString m_jsPreActionFunction{""};
    bool m_stopSoundAtAnimationEnd{false};
    bool m_finished{false};

    QVector<oxygine::spSingleResAnim> m_resAnims;
    /**
     * @brief animation data needed to perform this action
     */
    QBuffer m_buffer;
    QDataStream m_actionData{&m_buffer};
    QVector<oxygine::spTween> m_stageTweens;
};

Q_DECLARE_INTERFACE(GameAnimation, "GameAnimation");

#endif // GAMEANIMATION_H
