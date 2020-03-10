#ifndef DROPDOWNMENU_H
#define DROPDOWNMENU_H

#include <QObject>
#include <QVector>
#include <QString>
#include <oxygine-framework.h>

#include "objects/panel.h"
#include "objects/dropdownmenubase.h"

class DropDownmenu;
typedef oxygine::intrusive_ptr<DropDownmenu> spDropDownmenu;

class DropDownmenu : public DropDownmenuBase
{
    Q_OBJECT
public:
    explicit DropDownmenu(qint32 width, QVector<QString> items);
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
signals:
    void sigItemChanged(qint32 item);
public slots:
    virtual void itemChanged(qint32 item) override;
protected:
    void addDropDownText(QString text, qint32 id);
private:
    oxygine::spTextField m_Textfield;
    QVector<QString> m_ItemTexts;
};

#endif // DROPDOWNMENU_H
