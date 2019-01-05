#ifndef GAMEANIMATION_H
#define GAMEANIMATION_H

#include <QObject>

#include "oxygine-framework.h"

class GameAnimation : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit GameAnimation(quint32 frameTime);

signals:
    void sigFinished();
public slots:
    /**
     * @brief addSprite
     * @param spriteID
     */
    void addSprite(QString spriteID, float offsetX, float offsetY);

    virtual void onFinished();
private:
    quint32 m_frameTime{0};
};

#endif // GAMEANIMATION_H
