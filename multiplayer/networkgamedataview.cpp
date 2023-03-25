#include <QBuffer>

#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "coreengine/mainapp.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "multiplayer/networkgamedataview.h"

#include "objects/base/label.h"

NetworkGameDataView::NetworkGameDataView(NetworkGameData & data)
{
#ifdef GRAPHICSUPPORT
    setObjectName("NetworkGameDataView");
#endif
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    QImage img;
    getMinimapImage(img, data);
    m_minimapImage = oxygine::spSingleResAnim::create();
    Mainapp::getInstance()->loadResAnim(m_minimapImage, img, 1, 1, 1, false);



    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont48());
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = false;
    spLabel pLabel = spLabel::create(Settings::getWidth() - 100);
    pLabel->setStyle(style);
    pLabel->setX(50);
    pLabel->setY(10);
    pLabel->setText(tr("Game info"));



    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(Settings::getWidth() / 2 - pOkButton->getWidth() / 2, Settings::getHeight() - 10 - pOkButton->getScaledHeight());
    pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFinished();
    });
    connect(this, &NetworkGameDataView::sigFinished, this, &NetworkGameDataView::remove, Qt::QueuedConnection);
}

void NetworkGameDataView::remove()
{
    detach();
}

void NetworkGameDataView::getMinimapImage(QImage & img, NetworkGameData & data)
{
    QBuffer buffer(&data.getMinimapData());
    buffer.open(QIODevice::ReadOnly);
    img.load(&buffer, "PNG");
}
