#include "dropdownmenu.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DropDownmenu::DropDownmenu(qint32 width, QVector<QString> items)
    : DropDownmenuBase(width, items.size()),
      m_ItemTexts(items)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setWidth(width);
    m_Textfield = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_Textfield->setStyle(style);
    changeList(items);
}

void DropDownmenu::changeList(QVector<QString> items)
{
    m_Items.clear();
    m_Panel->clearContent();
    if (items.size() > 0)
    {
        m_Textfield->setHtmlText(items[0]);
    }
    else
    {
        m_Textfield->setHtmlText(tr("unknown"));
    }
    m_pClipActor->addChild(m_Textfield);
    m_Textfield->setWidth(m_Box->getWidth() - 20 - 45);
    m_Textfield->setHeight(m_Box->getHeight());
    m_Textfield->setY(5);

    qint32 scrollHeigth = 6 * 40;
    if (items.size() < 6)
    {
        scrollHeigth = (items.size() + 1) * 40;
    }

    for (qint32 i = 0; i < m_ItemTexts.size(); i++)
    {
        addDropDownText(m_ItemTexts[i], i);
    }
    changeItemCount(items.size());
}

void DropDownmenu::setCurrentItem(qint32 index)
{
    if ((index >= 0) && (index < m_ItemTexts.size()))
    {
        m_currentItem = index;
        m_Textfield->setHtmlText(m_ItemTexts[index]);
        
    }
}

void DropDownmenu::setCurrentItem(QString item)
{
    for (qint32 i = 0; i < m_ItemTexts.size(); i++)
    {
        if (m_ItemTexts[i] == item)
        {
            setCurrentItem(i);
            break;
        }
    }
}

QString DropDownmenu::getCurrentItemText()
{
    return m_Textfield->getText();
}

void DropDownmenu::setCurrentItemText(QString value)
{
    
    m_Textfield->setHtmlText(value);
    m_currentItem = -1;
    hideTooltip();
    
}

void DropDownmenu::addDropDownText(QString text, qint32 id)
{
    oxygine::spClipRectActor clipRect = new oxygine::ClipRectActor();
    oxygine::spTextField textField = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_DEFAULT;
    style.multiline = false;
    textField->setStyle(style);
    textField->setHtmlText(text);
    textField->setY(5);
    clipRect->addChild(textField);
    clipRect->setSize(m_Textfield->getWidth() + 10, m_Textfield->getHeight());
    clipRect->setX(10);
    auto size = addDropDownItem(clipRect.get(), id);
    textField->setSize(size);
}

void DropDownmenu::itemChanged(qint32 item)
{
    
    m_Textfield->setHtmlText(m_ItemTexts[item]);
    emit sigItemChanged(m_currentItem);
    
}
