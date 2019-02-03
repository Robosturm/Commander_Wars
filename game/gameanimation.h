#ifndef GAMEANIMATION_H
#define GAMEANIMATION_H

#include <QObject>

#include <QVector>

#include "oxygine-framework.h"

#include "game/smartpointers.h"

class GameAnimation : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit GameAnimation(quint32 frameTime);

signals:
    void sigFinished();
public slots:
    /**
     * @brief queueAnimation
     * @param pGameAnimation
     */
    void queueAnimation(GameAnimation* pGameAnimation);
    /**
     * @brief addSprite
     * @param spriteID
     */
    void addSprite(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish = 0);
    /**
     * @brief onFinished called when the animation is finished
     */
    virtual void onFinished();
    /**
     * @brief setEndOfAnimationCall calls a java script function when the animation is finished. Note: no parameters can be used to call the function
     * @param postActionObject java script object that will be used
     * @param postActionFunction java script function of the object that will be called
     */
    void setEndOfAnimationCall(QString postActionObject, QString postActionFunction);
private:
    quint32 m_frameTime{0};
    QVector<GameAnimation*> m_QueuedAnimations;
    QString jsPostActionObject{""};
    QString jsPostActionFunction{""};
};

#endif // GAMEANIMATION_H
