#ifndef DROPDOWNMENU_H
#define DROPDOWNMENU_H

#include <QObject>
#include <QVector>
#include <QString>
#include <oxygine-framework.h>

#include "objects/panel.h"

class DropDownmenu;
typedef oxygine::intrusive_ptr<DropDownmenu> spDropDownmenu;

class DropDownmenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DropDownmenu(qint32 width, QVector<QString> items, bool up = false);


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
    void setCurrentItemText(QString value);

    void setCurrentItem(qint32 index);

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
    oxygine::spTextField m_Textfield;
    oxygine::spButton m_pArrowDown;
    QVector<QString> m_ItemTexts;
    QVector<oxygine::spBox9Sprite> m_Items;
    spPanel m_Panel;
    qint32 m_currentItem{0};

    void addDropDownItem(QString text, qint32 id);
};

#endif // DROPDOWNMENU_H
