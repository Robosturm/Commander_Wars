#ifndef DROPDOWNMENU_H
#define DROPDOWNMENU_H

#include <QObject>
#include <QVector>
#include <QString>

#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"

#include "objects/base/panel.h"
#include "objects/base/dropdownmenubase.h"

class DropDownmenu;
using spDropDownmenu = oxygine::intrusive_ptr<DropDownmenu>;

class DropDownmenu : public DropDownmenuBase
{
    Q_OBJECT
public:
    explicit DropDownmenu(qint32 width, const QStringList & items);
    virtual ~DropDownmenu() = default;
    /**
     * @brief getCurrentItemText text of the current item
     * @return
     */
    QString getCurrentItemText();
    /**
     * @brief setCurrentItemText
     * @param value
     */
    void setCurrentItemText(QString value);
    /**
     * @brief setCurrentItem
     * @param item
     */
    void setCurrentItem(QString item);
    /**
     * @brief setCurrentItem
     * @param index
     */
    void setCurrentItem(qint32 index) override;
    /**
     * @brief changeList
     * @param items
     */
    void changeList(const QStringList & items);
signals:
    void sigItemChanged(qint32 item);
public slots:
    virtual void itemChanged(qint32 item) override;
protected:
    void addDropDownText(QString text, qint32 id);
private:
    oxygine::spTextField m_Textfield;
    QStringList m_ItemTexts;
};

#endif // DROPDOWNMENU_H
