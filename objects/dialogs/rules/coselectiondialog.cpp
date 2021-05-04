#include "coselectiondialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/cospritemanager.h"

#include "objects/dialogs/ingame/coinfodialog.h"

#include "game/co.h"


COSelectionDialog::COSelectionDialog(QString coid, QColor color, qint32 player, QStringList coids)
    : QObject(),
      m_player(player)
{
    setObjectName("COSelectionDialog");
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

    m_COSelection = spCOSelection::create(QSize(Settings::getWidth() - 60, Settings::getHeight() - 100), coids);
    m_COSelection->colorChanged(color);
    m_COSelection->setPosition(30, 30);
    pSpriteBox->addChild(m_COSelection);
    selectedCOIDChanged(coid);

    connect(m_COSelection.get(), &COSelection::coSelected, this, &COSelectionDialog::selectedCOIDChanged);
    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() - m_OkButton->getWidth() - 30, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit editFinished(m_currentCOID, m_player);
        detach();
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit canceled();
        detach();
    });

    // show co info button
    m_ShowCOInfoButton = pObjectManager->createButton(tr("Show CO Info"), 250);
    m_ShowCOInfoButton->setPosition(Settings::getWidth() / 2 - m_ShowCOInfoButton->getWidth() / 2, Settings::getHeight() - 30 - m_ShowCOInfoButton->getHeight());
    pSpriteBox->addChild(m_ShowCOInfoButton);
    m_ShowCOInfoButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowCOInfo();
    });
    connect(this, &COSelectionDialog::sigShowCOInfo, this, &COSelectionDialog::showCOInfo, Qt::QueuedConnection);
}

void COSelectionDialog::showCOInfo()
{
    QString coid = m_currentCOID;
    if (coid.isEmpty())
    {
        coid = COSpriteManager::getInstance()->getID(0);
    }
    Player* pPlayer = GameMap::getInstance()->getPlayer(m_player);
    spCO co = spCO::create(coid, pPlayer);
    addChild(spCOInfoDialog::create(co, pPlayer, [=](spCO& pCurrentCO, spPlayer&, qint32 direction)
    {
        COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
        qint32 index = pCOSpriteManager->getIndex(pCurrentCO->getCoID());
        index += direction;
        QString coid;
        if (index < 0)
        {
            coid = pCOSpriteManager->getID(pCOSpriteManager->getCount() - 1);
        }
        else if (index >= pCOSpriteManager->getCount())
        {
            coid = pCOSpriteManager->getID(0);
        }
        else
        {
            coid = pCOSpriteManager->getID(index);
        }
        pCurrentCO = spCO::create(coid, pPlayer);
    }, false));
}

void COSelectionDialog::selectedCOIDChanged(QString coid)
{
    
    m_currentCOID = coid;
    
}
