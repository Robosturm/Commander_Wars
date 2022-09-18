#ifndef GENERICBOX_H
#define GENERICBOX_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

class GenericBox;
using spGenericBox = oxygine::intrusive_ptr<GenericBox>;

class GenericBox final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit GenericBox(bool cancel = false);
    ~GenericBox() = default;
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
private slots:
    void remove();
private:
    oxygine::spBox9Sprite m_pSpriteBox;
};

#endif // GENERICBOX_H
