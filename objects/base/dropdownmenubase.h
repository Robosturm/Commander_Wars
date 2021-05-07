#ifndef DROPDOWNMENUBASE_H
#define DROPDOWNMENUBASE_H

#include <QObject>
#include <QVector>
#include <QString>
#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"
#include "objects/base/tooltip.h"

class DropDownmenuBase;
typedef oxygine::intrusive_ptr<DropDownmenuBase> spDropDownmenuBase;

class DropDownmenuBase : public Tooltip
{
    Q_OBJECT
public:
    explicit DropDownmenuBase(qint32 width, qint32 itemcount);


    /**
     * @brief getCurrentItem index of the current item
     * @return
     */
    qint32 getCurrentItem() const;
    /**
     * @brief setCurrentItem
     * @param index
     */
    virtual void setCurrentItem(qint32 index) = 0;
    /**
     * @brief getItemCount
     * @return
     */
    inline qint32 getItemCount()
    {
        return m_Items.size();
    }
    /**
     * @brief setEnabled
     * @param value
     */
    virtual void setEnabled(bool value) override;
    /**
     * @brief changeItemCount
     * @param itemcount
     */
    void changeItemCount(qint32 itemcount);
signals:
    void sigItemChangedInternal(qint32 item);

    void sigShowDropDown();
    void sigHideDropDown();
public slots:
    virtual void itemChanged(qint32 item) = 0;

public slots:
    void showDropDown();
    void hideDropDown();
    virtual void focusedLost() override;
protected:
    const oxygine::Vector2& addDropDownItem(oxygine::spActor item, qint32 id);
protected:
    oxygine::spClipRectActor m_pClipActor;
    oxygine::spBox9Sprite m_Box;
    oxygine::spButton m_pArrowDown;
    QVector<oxygine::spBox9Sprite> m_Items;
    spPanel m_Panel;
    qint32 m_currentItem{0};
    oxygine::spActor m_OriginalOwner;
    oxygine::Vector2 m_OriginalPosition;
};

#endif // DROPDOWNMENU_H
