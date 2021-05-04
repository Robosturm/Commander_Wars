#include "dropdownmenucolor.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/dialogs/colorselectiondialog.h"

DropDownmenuColor::DropDownmenuColor(qint32 width, QVector<QColor> items)
    : DropDownmenuBase(width, items.size()),
      m_ItemColors(items)
{
    setObjectName("DropDownmenuColor");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setWidth(width);
    m_Colorfield = oxygine::spColorRectSprite::create();
    m_Colorfield->addClickListener([=](oxygine::Event* event)
    {
        event->stopPropagation();
        looseFocusInternal();
        emit sigHideDropDown();
        emit sigShowColorDialog();
    });
    connect(this, &DropDownmenuColor::sigShowColorDialog, this, &DropDownmenuColor::showColorDialog, Qt::QueuedConnection);
    m_Colorfield->setWidth(m_Box->getWidth() - 20 - 45);
    m_Colorfield->setHeight(m_Box->getHeight() - 20);
    m_Colorfield->setColor(m_ItemColors[0].red(), m_ItemColors[0].green(), m_ItemColors[0].blue(), 255);
    m_Colorfield->setDisableColor(QColor(0, 0, 0, 0));
    m_Colorfield->setY(10);
    m_pClipActor->addChild(m_Colorfield);

    for (qint32 i = 0; i < m_ItemColors.size(); i++)
    {
        addDropDownColor(m_ItemColors[i], i);
    }
}


void DropDownmenuColor::changeCurrentItem(QColor color)
{
    setCurrentItem(color);
    m_currentItem = -1;
    emit sigItemChanged(m_currentColor);
}

void DropDownmenuColor::setCurrentItem(qint32 index)
{
    setCurrentItem(m_ItemColors[index]);
}

void DropDownmenuColor::setCurrentItem(QColor color)
{
    
    m_currentColor = color;
    hideTooltip();
    m_Colorfield->setColor(color.red(), color.green(), color.blue(), 255);
    
}

QColor DropDownmenuColor::getCurrentItemColor()
{
    return m_currentColor;
}

void DropDownmenuColor::addDropDownColor(QColor color, qint32 id)
{
    oxygine::spColorRectSprite colorField = oxygine::spColorRectSprite::create();
    colorField->setColor(color.red(), color.green(), color.blue(), 255);
    auto size = addDropDownItem(colorField, id);
    colorField->setSize(size.x - 20, size.y - 20);
    colorField->setPosition(10, 10);
}

void DropDownmenuColor::itemChanged(qint32 item)
{
    
    m_currentColor = m_ItemColors[item];
    m_Colorfield->setColor(m_currentColor.red(), m_currentColor.green(), m_currentColor.blue(), 255);
    emit sigItemChanged(m_currentColor);
    
}

void DropDownmenuColor::showColorDialog()
{
    if (getEnabled())
    {
        spColorSelectionDialog pDialog = spColorSelectionDialog::create(m_currentColor);
        oxygine::getStage()->addChild(pDialog);
        connect(pDialog.get(), &ColorSelectionDialog::editFinished, this, &DropDownmenuColor::changeCurrentItem, Qt::QueuedConnection);
        
    }
}
