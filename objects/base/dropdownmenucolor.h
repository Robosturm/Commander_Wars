#ifndef DROPDOWNMENUCOLOR_H
#define DROPDOWNMENUCOLOR_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QColor>
#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"
#include "objects/base/dropdownmenubase.h"

class DropDownmenuColor;
typedef oxygine::intrusive_ptr<DropDownmenuColor> spDropDownmenuColor;

class DropDownmenuColor : public DropDownmenuBase
{
    Q_OBJECT
public:
    explicit DropDownmenuColor(qint32 width, QVector<QColor> items);

    /**
     * @brief getCurrentItem color of the current item
     * @return
     */
    QColor getCurrentItemColor();

    void setCurrentItem(QColor color);
    /**
     * @brief setCurrentItem
     * @param index
     */
    virtual void setCurrentItem(qint32 index) override;
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
    /**
     * @brief itemChanged
     * @param item
     */
    virtual void itemChanged(qint32 item) override;
protected:
    void addDropDownColor(QColor color, qint32 id);
private:
    oxygine::spColorRectSprite m_Colorfield;
    QVector<QColor> m_ItemColors;
    QColor m_currentColor;
};

#endif // DropDownmenuColor_H
