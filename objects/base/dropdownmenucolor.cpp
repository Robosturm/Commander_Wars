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
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setWidth(width);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("dropdownmenu");

    pAnim = pObjectManager->getResAnim("dropdownmenucolor");
    m_Colorfield = new oxygine::Sprite();
    m_Colorfield->setResAnim(pAnim);
    m_Colorfield->addClickListener([=](oxygine::Event*)
    {
        emit sigShowColorDialog();
    });
    connect(this, &DropDownmenuColor::sigShowColorDialog, this, &DropDownmenuColor::showColorDialog, Qt::QueuedConnection);
    m_Colorfield->setWidth(pAnim->getWidth());
    m_Colorfield->setHeight(pAnim->getHeight());
    m_Colorfield->setScaleX((m_Box->getWidth() - 20 - 45) / pAnim->getWidth());
    m_Colorfield->setScaleY((m_Box->getHeight() - 20) / pAnim->getHeight());
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
    oxygine::spColorRectSprite colorField = new oxygine::ColorRectSprite();
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
        spColorSelectionDialog pDialog = new ColorSelectionDialog(m_currentColor);
        oxygine::getStage()->addChild(pDialog);
        connect(pDialog.get(), &ColorSelectionDialog::editFinished, this, &DropDownmenuColor::changeCurrentItem, Qt::QueuedConnection);
        
    }
}
