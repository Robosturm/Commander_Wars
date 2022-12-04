#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/base/dropdownmenucolor.h"
#include "objects/dialogs/colorselectiondialog.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DropDownmenuColor::DropDownmenuColor(qint32 width, QVector<QColor> items)
    : DropDownmenuBase(width, items.size()),
      m_ItemColors(items)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DropDownmenuColor");
#endif
    Interpreter::setCppOwnerShip(this);

    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setWidth(width);

    oxygine::spColorRectSprite colorField = oxygine::spColorRectSprite::create();
    colorField->setColor(Qt::black);
    colorField->setWidth(m_pClipActor->getScaledWidth() - 10);
    colorField->setHeight(m_pClipActor->getScaledHeight() - 13);
    colorField->setY(7);
    m_pClipActor->addChild(colorField);

    m_Colorfield = oxygine::spColorRectSprite::create();
    if (!Settings::getSmallScreenDevice())
    {
        m_Colorfield->addClickListener([this](oxygine::Event* event)
        {
            event->stopPropagation();
            looseFocusInternal();
            emit sigHideDropDown();
            emit sigShowColorDialog();
        });
    }
    connect(this, &DropDownmenuColor::sigShowColorDialog, this, &DropDownmenuColor::showColorDialog, Qt::QueuedConnection);
    m_Colorfield->setWidth(m_pClipActor->getScaledWidth() - 16);
    m_Colorfield->setHeight(m_pClipActor->getScaledHeight() - 19);
    m_Colorfield->setColor(m_ItemColors[0].red(), m_ItemColors[0].green(), m_ItemColors[0].blue(), 255);
    m_Colorfield->setDisableColor(QColor(0, 0, 0, 0));
    m_Colorfield->setY(10);
    m_Colorfield->setX(3);
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
    if (index >= 0 && index < m_ItemColors.size())
    {
        m_currentItem = index;
        setCurrentItem(m_ItemColors[index]);
    }
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
    oxygine::spColorRectSprite colorField1 = oxygine::spColorRectSprite::create();
    colorField1->setColor(Qt::black);
    oxygine::spColorRectSprite colorField2 = oxygine::spColorRectSprite::create();
    colorField2->setColor(color.red(), color.green(), color.blue(), 255);
    colorField1->addChild(colorField2);
    auto size = addDropDownItem(colorField1, id);
    colorField1->setSize(size.x - 14, size.y - 14);
    colorField1->setPosition(7, 7);
    colorField2->setSize(size.x - 20, size.y - 20);
    colorField2->setPosition(3, 3);
}

bool DropDownmenuColor::getShowUnitPreview() const
{
    return m_showUnitPreview;
}

void DropDownmenuColor::setShowUnitPreview(bool newShowUnitPreview)
{
    m_showUnitPreview = newShowUnitPreview;
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
        spColorSelectionDialog pDialog = spColorSelectionDialog::create(m_currentColor, m_showUnitPreview);
        oxygine::Stage::getStage()->addChild(pDialog);
        connect(pDialog.get(), &ColorSelectionDialog::editFinished, this, &DropDownmenuColor::changeCurrentItem, Qt::QueuedConnection);
        
    }
}
