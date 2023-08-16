#ifndef DROPDOWNMENUCOLOR_H
#define DROPDOWNMENUCOLOR_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QColor>

#include "objects/base/dropdownmenubase.h"

class DropDownmenuColor;
using spDropDownmenuColor = std::shared_ptr<DropDownmenuColor>;

class DropDownmenuColor final : public DropDownmenuBase
{
    Q_OBJECT
public:
    explicit DropDownmenuColor(qint32 width, QVector<QColor> items);
   virtual ~DropDownmenuColor() = default;
    /**
     * @brief getCurrentItem color of the current item
     * @return
     */
    Q_INVOKABLE QColor getCurrentItemColor();
    Q_INVOKABLE void setCurrentItem(QColor color);
    /**
     * @brief setCurrentItem
     * @param index
     */
    Q_INVOKABLE virtual void setCurrentItem(qint32 index) override;
    Q_INVOKABLE bool getShowUnitPreview() const;
    Q_INVOKABLE void setShowUnitPreview(bool newShowUnitPreview);
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
    bool m_showUnitPreview{false};
};

Q_DECLARE_INTERFACE(DropDownmenuColor, "DropDownmenuColor");

#endif // DropDownmenuColor_H
