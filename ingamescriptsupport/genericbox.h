#ifndef GENERICBOX_H
#define GENERICBOX_H

#include <QObject>

#include "objects/base/panel.h"

class GenericBox;
using spGenericBox = oxygine::intrusive_ptr<GenericBox>;

class GenericBox final : public QObject, public oxygine::Actor
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
    /**
     * @brief clearAllItems
     */
    void clearAllItems();
    /**
     * @brief setContentSize
     * @param width
     * @param height
     */
    void setContentSize(qint32 width, qint32 height);
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
    spPanel m_pPanel;
};

#endif // GENERICBOX_H
