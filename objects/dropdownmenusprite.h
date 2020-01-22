#ifndef DROPDOWNMENUSPRITE_H
#define DROPDOWNMENUSPRITE_H

#include <QObject>
#include <QVector>
#include <QString>
#include <oxygine-framework.h>

#include "objects/panel.h"
#include "objects/tooltip.h"

class DropDownmenuSprite;
typedef oxygine::intrusive_ptr<DropDownmenuSprite> spDropDownmenuSprite;

class DropDownmenuSprite : public Tooltip
{
    Q_OBJECT
public:
    explicit DropDownmenuSprite(qint32 width, QVector<QString>& items, std::function<oxygine::spActor(QString item)> creator, bool up = false, qint32 dropDownWidth = -1);


    /**
     * @brief getCurrentItem index of the current item
     * @return
     */
    qint32 getCurrentItem() const;
    /**
     * @brief getCurrentItemText text of the current item
     * @return
     */
    QString getCurrentItemText();
    /**
     * @brief setCurrentItem
     * @param item
     */
    void setCurrentItem(QString item);
    /**
     * @brief setCurrentItem
     * @param index
     */
    void setCurrentItem(qint32 index);
    /**
     * @brief getItemCount
     * @return
     */
    inline qint32 getItemCount()
    {
        return m_ItemTexts.size();
    }


    virtual void setEnabled(bool value) override;
signals:
    void sigItemChanged(qint32 item);
public slots:

private:
    oxygine::spBox9Sprite m_Textbox;
    oxygine::spClipRectActor m_pClipActor;
    oxygine::spButton m_pArrowDown;
    QVector<QString> m_ItemTexts;
    std::function<oxygine::spActor(QString item)> m_Creator;
    QVector<oxygine::spBox9Sprite> m_Items;
    QString m_currentText;
    spPanel m_Panel;
    qint32 m_currentItem{0};

    void addDropDownItem(QString spriteID, qint32 id, qint32 dropDownWidth);
};

#endif // DROPDOWNMENUSPRITE_H
