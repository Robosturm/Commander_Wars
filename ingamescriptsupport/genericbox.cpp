#include "ingamescriptsupport/genericbox.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

GenericBox::GenericBox(bool cancel)
    : QObject()
{
    setObjectName("GenericBox");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    m_pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    m_pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));


    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(Settings::getWidth() / 2 - pOkButton->getWidth() / 2, Settings::getHeight() - 10 - pOkButton->getHeight());
    m_pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        emit sigFinished();
    });
    if (cancel)
    {
        pOkButton->setX(Settings::getWidth() / 2 + 10);
        oxygine::spButton pCancelButton = pObjectManager->createButton(tr("Cancel"), 150);
        pCancelButton->setPosition(Settings::getWidth() / 2 - pCancelButton->getWidth() - 10, Settings::getHeight() - 10 - pOkButton->getHeight());
        m_pSpriteBox->addChild(pCancelButton);
        pCancelButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
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

