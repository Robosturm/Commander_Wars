#include "coinfodialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/cospritemanager.h"

#include "resource_management/unitspritemanager.h"

#include "resource_management/gamemanager.h"

#include "objects/base/panel.h"

#include "game/gamemap.h"

COInfoDialog::COInfoDialog(spCO pCO, spPlayer pPlayer,
                           std::function<void (spCO& m_CurrrentCO, spPlayer& pPlayer, qint32 direction)> stepFunction,
                           bool ingame)
    : QObject(),
      m_CurrentCO(pCO),
      m_pCurrentPlayer(pPlayer),
      m_StepFunction(stepFunction),
      m_Ingame(ingame)
{
    setObjectName("COInfoDialog");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // next button
    m_NextButton = pObjectManager->createButton(tr("Next"), 150);
    m_NextButton->setPosition(Settings::getWidth() - m_NextButton->getWidth() - 30, Settings::getHeight() - 30 - m_NextButton->getHeight());
    pSpriteBox->addChild(m_NextButton);
    m_NextButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit next();
    });

    // quit button
    m_QuitButton = pObjectManager->createButton(tr("Quit"), 150);
    m_QuitButton->setPosition(Settings::getWidth() / 2 - m_QuitButton->getWidth() / 2, Settings::getHeight() - 30 - m_QuitButton->getHeight());
    pSpriteBox->addChild(m_QuitButton);
    m_QuitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit quit();
        detach();
    });

    // back button
    m_BackButton = pObjectManager->createButton(tr("Back"), 150);
    m_BackButton->setPosition(30, Settings::getHeight() - 30 - m_BackButton->getHeight());
    pSpriteBox->addChild(m_BackButton);
    m_BackButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit back();
    });

    connect(this, &COInfoDialog::next, this, &COInfoDialog::slotNext, Qt::QueuedConnection);
    connect(this, &COInfoDialog::back, this, &COInfoDialog::slotBack, Qt::QueuedConnection);

    // no the fun begins create checkboxes and stuff and a panel down here
    m_pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 110),
                         QSize(Settings::getWidth() - 60, Settings::getHeight() - 110));
    m_pPanel->setPosition(30, 30);
    pSpriteBox->addChild(m_pPanel);
    m_COInfo = spCOInfoActor::create(m_pPanel->getWidth());
    m_pPanel->addItem(m_COInfo);
    showCO();
}

void COInfoDialog::slotNext()
{
    m_StepFunction(m_CurrentCO, m_pCurrentPlayer, 1);
    showCO();
}

void COInfoDialog::slotBack()
{
    m_StepFunction(m_CurrentCO, m_pCurrentPlayer, -1);
    showCO();
}

void COInfoDialog::showCO()
{
    
    m_COInfo->showCO(m_CurrentCO, m_pCurrentPlayer);
    m_pPanel->setContentHeigth(m_COInfo->getHeight());
    
}
