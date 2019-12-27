#ifndef DROPDOWNMENUCOLOR_H
#define DROPDOWNMENUCOLOR_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QColor>
#include <oxygine-framework.h>

#include "objects/panel.h"
#include "objects/tooltip.h"

class DropDownmenuColor;
typedef oxygine::intrusive_ptr<DropDownmenuColor> spDropDownmenuColor;

class DropDownmenuColor : public Tooltip
{
    Q_OBJECT
public:
    explicit DropDownmenuColor(qint32 width, QVector<QColor> items, bool up = false);

    /**
     * @brief getCurrentItem color of the current item
     * @return
     */
    QColor getCurrentItem();

    void setCurrentItem(QColor color);

    virtual void setEnabled(bool value) override;
signals:
    void sigItemChanged(QColor color);
    void sigShowColorDialog();
public slots:
    void showColorDialog();
    /**
     * @brief changeCurrentItem sets the current color and calls itemchanged
     * @param color
     */
    void changeCurrentItem(QColor color);
private:
    oxygine::spBox9Sprite m_Colorbox;
    oxygine::spSprite m_Colorfield;
    oxygine::spButton m_pArrowDown;
    QVector<QColor> m_ItemColors;
    QVector<oxygine::spBox9Sprite> m_Items;
    spPanel m_Panel;
    QColor m_currentItem;

    void addDropDownItem(QColor color, qint32 id);
};

#endif // DropDownmenuColor_H
