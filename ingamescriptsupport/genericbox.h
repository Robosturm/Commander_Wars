#ifndef GENERICBOX_H
#define GENERICBOX_H

#include <QObject>

#include "oxygine-framework.h"

class GenericBox;
typedef oxygine::intrusive_ptr<GenericBox> spGenericBox;

class GenericBox : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit GenericBox();
    virtual ~GenericBox() = default;
    /**
     * @brief addItem
     * @param pActor
     */
    void addItem(oxygine::spActor pActor);
signals:
    /**
     * @brief sigFinished
     */
    void sigFinished();
private:
    oxygine::spBox9Sprite m_pSpriteBox;
};

#endif // GENERICBOX_H
