#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "ingamescriptsupport/genericbox.h"

#include "resource_management/objectmanager.h"

#include "coreengine/interpreter.h"

GenericBox::GenericBox(bool cancel)
{
#ifdef GRAPHICSUPPORT
    setObjectName("GenericBox");
#endif
    Interpreter::setCppOwnerShip(this);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    QSize size(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight() - 100);
    m_pPanel = MemoryManagement::create<Panel>(true, size, size);
    m_pSpriteBox->addChild(m_pPanel);

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - pOkButton->getWidth() / 2, oxygine::Stage::getStage()->getHeight() - 10 - pOkButton->getScaledHeight());
    m_pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFinished();
    });
    if (cancel)
    {
        pOkButton->setX(oxygine::Stage::getStage()->getWidth() / 2 + 10);
        oxygine::spButton pCancelButton = pObjectManager->createButton(tr("Cancel"), 150);
        pCancelButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - pCancelButton->getScaledWidth() - 10,
                                   oxygine::Stage::getStage()->getHeight() - 10 - pOkButton->getScaledHeight());
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
    m_pPanel->addItem(pActor);
}

void GenericBox::clearAllItems()
{
    m_pPanel->clearContent();
}

void GenericBox::setContentSize(qint32 width, qint32 height)
{
    m_pPanel->setContentWidth(width);
    m_pPanel->setContentHeigth(height);
}
