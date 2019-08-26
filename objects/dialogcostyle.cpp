#include "dialogcostyle.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "objects/panel.h"

DialogCOStyle::DialogCOStyle(QString coid)
    : QObject()
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

    spPanel pPanel = new Panel(true, QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110),
                                     QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110));
    pPanel->setPosition(30, 30);
    pSpriteBox->addChild(pPanel);

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Apply"), 150);
    pOkButton->setPosition(pApp->getSettings()->getWidth() - pOkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - pOkButton->getHeight());
    pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {

        this->getParent()->removeChild(this);
        emit sigFinished();
    });

    // cancel button
    oxygine::spButton pExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    pExitButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - pExitButton->getHeight());
    pSpriteBox->addChild(pExitButton);
    pExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        this->getParent()->removeChild(this);
        emit sigCancel();
    });
}
