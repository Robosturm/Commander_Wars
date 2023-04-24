#ifndef DROPDOWNMENUSPRITE_H
#define DROPDOWNMENUSPRITE_H

#include <QObject>
#include <QVector>
#include <QString>

#include "objects/base/dropdownmenubase.h"

class DropDownmenuSprite;
using spDropDownmenuSprite = oxygine::intrusive_ptr<DropDownmenuSprite>;

class DropDownmenuSprite final : public DropDownmenuBase
{
    Q_OBJECT
public:
    explicit DropDownmenuSprite(qint32 width, QStringList& items, std::function<oxygine::spActor(QString item)> creator, qint32 dropDownWidth = -1, bool autoScale = true);
    ~DropDownmenuSprite() = default;
signals:
    void sigItemChanged(qint32 item);
    void sigItemString(QString item);

public slots:
    virtual void itemChanged(qint32 item) override;
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
    virtual void setCurrentItem(qint32 index) override;
protected:
    void addDropDownText(QString spriteID, qint32 id, qint32 dropDownWidth);
private:
    QStringList m_ItemTexts;
    std::function<oxygine::spActor(QString item)> m_Creator;
    QVector<oxygine::spBox9Sprite> m_Items;
    QString m_currentText;
    bool m_autoScale{true};

};

Q_DECLARE_INTERFACE(DropDownmenuSprite, "DropDownmenuSprite");

#endif // DROPDOWNMENUSPRITE_H
