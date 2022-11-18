#include "ingamescriptsupport/genericbox.h"

#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

GenericBox::GenericBox(bool cancel)
{
#ifdef GRAPHICSUPPORT
    setObjectName("GenericBox");
#endif
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));


    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(Settings::getWidth() / 2 - pOkButton->getWidth() / 2, Settings::getHeight() - 10 - pOkButton->getScaledHeight());
    m_pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFinished();
    });
    if (cancel)
    {
        pOkButton->setX(Settings::getWidth() / 2 + 10);
        oxygine::spButton pCancelButton = pObjectManager->createButton(tr("Cancel"), 150);
        pCancelButton->setPosition(Settings::getWidth() / 2 - pCancelButton->getScaledWidth() - 10,
                                   Settings::getHeight() - 10 - pOkButton->getScaledHeight());
        m_pSpriteBox->addChild(pCancelButton);
        pCancelButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
        {
            emit sigCancel();
        });
    }
    connect(this, &GenericBox::sigCancel, this, &GenericBox::remove, Qt::QueuedConnection);
    connect(this, &GenericBox::sigFinished, this, &GenericBox::remove, Qt::QueuedConnection);
}

void GenericBox::remove()
{
    detach();
}

void GenericBox::addItem(oxygine::spActor pActor)
{
    m_pSpriteBox->addChild(pActor);
}

