#ifndef TOPBAR_H
#define TOPBAR_H

#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

#include "objects/base/tooltip.h"
#include "objects/base/panel.h"
#include "objects/base/focusableobject.h"

class Topbar;
using spTopbar = std::shared_ptr<Topbar>;

class Topbar final : public FocusableObject
{
    Q_OBJECT
public:
    explicit Topbar(qint32 x, qint32 width);
   virtual ~Topbar() = default;
    /**
     * @brief addItem adds an item to the bar
     * @param text the text shown for this item
     * @param itemID the depending id used for event handling
     * @param group the used group item number
     */
    void addItem(QString text, QString itemID, qint32 group, QString tooltip);
    /**
     * @brief addGroup adds a group to the topbar
     * @param text
     */
    void addGroup(QString text);
    /**
     * @brief finishCreation
     */
    void finishCreation();
    /**
     * @brief hide
     */
    void hide();
signals:
    /**
     * @brief sigItemClicked connect to this signal in order to recieve when an item was clicked so you can handle it
     * @param itemID
     */
    void sigItemClicked(QString itemID);
public slots:
    virtual void focusedLost() override;
private:
    QVector<QVector<spTooltip>> m_Items;
    QVector<oxygine::spButton> m_Buttons;    
    QVector<spPanel> m_ItemPanels;
    oxygine::spBox9Sprite m_pSpriteBox;
};

#endif // TOPBAR_H
