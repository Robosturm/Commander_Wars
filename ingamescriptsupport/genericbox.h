#ifndef GENERICBOX_H
#define GENERICBOX_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class GenericBox;
typedef oxygine::intrusive_ptr<GenericBox> spGenericBox;

class GenericBox : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit GenericBox(bool cancel = false);
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
    /**
     * @brief sigCancel
     */
    void sigCancel();
private:
    oxygine::spBox9Sprite m_pSpriteBox;
};

#endif // GENERICBOX_H
