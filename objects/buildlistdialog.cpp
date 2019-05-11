#include "buildlistdialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/unitspritemanager.h"

#include "game/gamemap.h"

#include "objects/panel.h"

BuildListDialog::BuildListDialog(qint32 player, QStringList buildList)
    : QObject(),
      m_CurrentBuildList(buildList),
      m_Player(player)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() - m_OkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit editFinished(m_Player, m_CurrentBuildList);
        this->getParent()->removeChild(this);
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit canceled();
        this->getParent()->removeChild(this);
    });

    m_ToggleAll = pObjectManager->createButton(tr("Un/Select All"), 150);
    m_ToggleAll->setPosition(pApp->getSettings()->getWidth() / 2 - m_ToggleAll->getWidth() / 2 , pApp->getSettings()->getHeight() - 30 - m_ToggleAll->getHeight());
    pSpriteBox->addChild(m_ToggleAll);
    m_ToggleAll->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        toggle = !toggle;
        for (qint32 i = 0; i < m_Checkboxes.size(); i++)
        {
            m_Checkboxes[i]->setChecked(toggle);
            emit m_Checkboxes[i]->checkChanged(toggle);
        }
    });

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    // no the fun begins create checkboxes and stuff and a panel down here
    spPanel pPanel = new Panel(true, QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110),
                                     QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110));
    pPanel->setPosition(30, 30);
    pSpriteBox->addChild(pPanel);

    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setText(tr("Build List").toStdString().c_str());
    pLabel->setScale(2.0f);
    pLabel->setPosition(pPanel->getWidth() / 2 - pLabel->getTextRect().getWidth(), 10);
    pPanel->addItem(pLabel);

    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    qint32 y = 30 + pLabel->getTextRect().getHeight() * 2;
    qint32 x = 10;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pUnitSpriteManager->getUnitCount(); i++)
    {
        QString unitID = pUnitSpriteManager->getUnitID(i);

        spUnit pUnit = new Unit(unitID, pMap->getPlayer(player), false);

        pLabel = new oxygine::TextField();
        pLabel->setStyle(style);
        pLabel->setText(pUnit->getName().toStdString().c_str());

        pLabel->setPosition(x, y);
        pUnit->setPosition(x + 220 - GameMap::Imagesize * 1.25f - 10, y);
        pUnit->setScale(pUnit->getScale() * 1.25f);
        spCheckbox pCheckbox = new Checkbox();
        pCheckbox->setPosition(x + 220, y);
        m_Checkboxes.append(pCheckbox);

        if (m_CurrentBuildList.contains(unitID))
        {
            pCheckbox->setChecked(true);
        }
        else
        {
            pCheckbox->setChecked(false);
        }
        connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool checked)
        {
            if (checked)
            {
                m_CurrentBuildList.append(unitID);
            }
            else
            {
                m_CurrentBuildList.removeOne(unitID);
            }
        });

        pPanel->addItem(pCheckbox);
        pPanel->addItem(pLabel);
        pPanel->addItem(pUnit);

        if (x + 320 > pPanel->getContentWidth())
        {
            y += 40;
            x = 10;
            pLabel->setPosition(x, y);
            pUnit->setPosition(x + 220 - GameMap::Imagesize * 1.25f - 10, y);
            pCheckbox->setPosition(x + 220, y);
        }
        x += 280;
    }
    pPanel->setContentHeigth(y);
}
