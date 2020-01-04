#include "dropdownmenucolor.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/colorselectiondialog.h"

DropDownmenuColor::DropDownmenuColor(qint32 width, QVector<QColor> items, bool up)
    : m_ItemColors(items)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setWidth(width);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("dropdownmenu");
    m_Colorbox = new oxygine::Box9Sprite();
    m_Colorbox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_Colorbox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_Colorbox->setResAnim(pAnim);

    pAnim = pObjectManager->getResAnim("dropdownmenucolor");
    m_Colorfield = new oxygine::Sprite();
    m_Colorfield->setResAnim(pAnim);
    m_Colorfield->addClickListener([=](oxygine::Event*)
    {
        emit sigShowColorDialog();
    });
    connect(this, &DropDownmenuColor::sigShowColorDialog, this, &DropDownmenuColor::showColorDialog, Qt::QueuedConnection);
    oxygine::spClipRectActor pClipActor = new oxygine::ClipRectActor();
    m_Colorfield->attachTo(pClipActor);
    m_Colorbox->addChild(pClipActor);
    m_Colorbox->setSize(width, 40);
    m_Colorfield->setWidth(pAnim->getWidth());
    m_Colorfield->setHeight(pAnim->getHeight());
    m_Colorfield->setScaleX((m_Colorbox->getWidth() - 20 - 45) / pAnim->getWidth());
    m_Colorfield->setScaleY((m_Colorbox->getHeight() - 20) / pAnim->getHeight());
    m_Colorfield->setColor(m_ItemColors[0].red(), m_ItemColors[0].green(), m_ItemColors[0].blue(), 255);
    m_Colorfield->setDisableColor(QColor(0, 0, 0, 0));
    pClipActor->setWidth(m_Colorfield->getScaledWidth());
    pClipActor->setHeight(m_Colorfield->getScaledHeight());
    pClipActor->setX(10);
    pClipActor->setY(10);
    this->addChild(m_Colorbox);

    qint32 scrollHeigth = 6 * 40;
    if (items.size() < 6)
    {
        scrollHeigth = (items.size() + 1) * 40;
    }
    m_Panel = new Panel(false, QSize(width, scrollHeigth), QSize(width, items.size() * 40));
    m_Panel->setVisible(false);
    if (up)
    {
        m_Panel->setY(-m_Panel->getHeight());
    }
    else
    {
        m_Panel->setY(m_Colorbox->getHeight());
    }
    this->addChild(m_Panel);


    m_pArrowDown = new oxygine::Button();
    m_Colorbox->addChild(m_pArrowDown);
    m_pArrowDown->setPosition(m_Colorbox->getWidth() - 45, 10);
    // pButton->setPosition(200, 200);
    m_pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+down"));
    m_pArrowDown->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        if (m_Panel->getVisible())
        {
            this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
        }
        else
        {
            this->setPriority(static_cast<short>(Mainapp::ZOrder::FocusedObjects));
        }

        this->m_Panel->setVisible(!this->m_Panel->getVisible());
    });
    this->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        if (m_Panel->getVisible())
        {
            this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
        }
        this->m_Panel->setVisible(false);
    });
    for (qint32 i = 0; i < m_ItemColors.size(); i++)
    {
        addDropDownItem(m_ItemColors[i], i);
    }
}


void DropDownmenuColor::setEnabled(bool value)
{
    oxygine::Actor::setEnabled(value);
    m_pArrowDown->setEnabled(value);
    m_Colorbox->setEnabled(value);
    m_Colorfield->setEnabled(value);
    m_Panel->setVisible(false);
}

void DropDownmenuColor::changeCurrentItem(QColor color)
{
    setCurrentItem(color);
    emit sigItemChanged(m_currentItem);
}

void DropDownmenuColor::setCurrentItem(QColor color)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_currentItem = color;
    hideTooltip();
    m_Colorfield->setColor(color.red(), color.green(), color.blue(), 255);
    pApp->continueThread();
}

QColor DropDownmenuColor::getCurrentItem()
{
    return m_currentItem;
}

void DropDownmenuColor::addDropDownItem(QColor color, qint32 id)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("topbar+dropdown");
    oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setResAnim(pAnim);
    pBox->setSize(getWidth() - 33, 40);

    oxygine::spSprite colorField = new oxygine::Sprite();
    pAnim = pObjectManager->getResAnim("dropdownmenucolor");
    colorField->setResAnim(pAnim);
    colorField->setColor(color.red(), color.green(), color.blue(), 255);
    colorField->setSize(pAnim->getWidth(), pAnim->getHeight());
    colorField->setScaleX((pBox->getSize().x - 20) / pAnim->getWidth());
    colorField->setScaleY((pBox->getSize().y - 20) / pAnim->getHeight());
    oxygine::spClipRectActor pClipActor = new oxygine::ClipRectActor();
    pClipActor->addChild(colorField);
    pClipActor->setWidth(colorField->getScaledWidth());
    pClipActor->setHeight(colorField->getScaledHeight());
    pClipActor->setX(10);
    pClipActor->setY(10);

    pClipActor->attachTo(pBox);

    pBox->setPosition(0, 40 * m_Items.size());
    m_Items.append(pBox);
    pBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->m_Panel->addItem(pBox);
    // add some event handling :)
    pBox->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
    });
    pBox->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pBox->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        m_currentItem = color;
        this->m_Panel->setVisible(false);
        m_Colorfield->setColor(color.red(), color.green(), color.blue(), 255);
        emit sigItemChanged(m_currentItem);
    });
}

void DropDownmenuColor::showColorDialog()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    spColorSelectionDialog pDialog = new ColorSelectionDialog(m_currentItem);
    oxygine::getStage()->addChild(pDialog);
    connect(pDialog.get(), &ColorSelectionDialog::editFinished, this, &DropDownmenuColor::changeCurrentItem, Qt::QueuedConnection);
    pApp->continueThread();
}
