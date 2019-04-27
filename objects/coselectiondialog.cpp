#include "coselectiondialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/cospritemanager.h"

COSelectionDialog::COSelectionDialog(QString coid, QColor color, qint32 player)
    : QObject(),
      m_player(player)
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

    m_COSelection = new COSelection();
    m_COSelection->colorChanged(color);
    m_COSelection->setPosition(30, 30);
    m_COSelection->setScale((pSpriteBox->getSize().x - 70) / (m_COSelection->getWidth() + 208));
    pSpriteBox->addChild(m_COSelection);

    m_pCurrentCO = new oxygine::Sprite();
    m_pCurrentCO->setPosition(40 + m_COSelection->getScaledWidth(), 30);
    m_pCurrentCO->setScale(m_COSelection->getScale());
    pSpriteBox->addChild(m_pCurrentCO);
    selectedCOIDChanged(coid);

    connect(m_COSelection.get(), &COSelection::coSelected, this, &COSelectionDialog::selectedCOIDChanged);
    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() - m_OkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit editFinished(m_currentCOID, m_player);
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
}

void COSelectionDialog::selectedCOIDChanged(QString coid)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = nullptr;
    if (!coid.isEmpty())
    {
        pAnim = pCOSpriteManager->getResAnim((coid + "+nrm").toStdString().c_str());
    }
    m_pCurrentCO->setResAnim(pAnim);
    m_currentCOID = coid;
    pApp->continueThread();
}
