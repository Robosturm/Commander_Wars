#include <QBuffer>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "objects/base/spriteobject.h"

#include "multiplayer/networkgamedataview.h"

#include "ui_reader/uifactory.h"

NetworkGameDataView::NetworkGameDataView(NetworkGameData & data)
    : m_data(data)
{
#ifdef GRAPHICSUPPORT
    setObjectName("NetworkGameDataView");
#endif
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    QImage img;
    getMinimapImage(img, data);
    m_minimapImage = oxygine::spSingleResAnim::create();
    Mainapp::getInstance()->loadResAnim(m_minimapImage, img, 1, 1, 1);
    UiFactory::getInstance().createUi("ui/multiplayer/networkGameDataView.xml", this);
}

oxygine::spActor NetworkGameDataView::loadCustomId(const QString & item, qint32 x, qint32 y, bool enabled, bool visible, float scale,
                                                   const QString & id, const QString & tooltip, const QString & onEvent,
                                                   UiFactory* pFactoty, CreatedGui* pMenu, qint32 loopIdx, qint32 & scaledWidth, qint32 & scaledHeight)
{
    if (item == "MINIMAP")
    {
        spSpriteObject pSprite = spSpriteObject::create();
        pSprite->setObjectName(id);
        pSprite->setResAnim(m_minimapImage.get());
        pSprite->setScale(scale);
        pSprite->setPosition(x, y);
        pSprite->setVisible(visible);
        pSprite->setEnabled(enabled);
        pSprite->setSize(m_minimapImage->getSize() * scale);
        scaledWidth = m_minimapImage->getWidth() * scale;
        scaledHeight = m_minimapImage->getHeight() * scale;
        if (!onEvent.isEmpty())
        {
            pSprite->addClickListener([pFactoty, onEvent, id, loopIdx, pMenu](oxygine::Event*)
            {
                emit pFactoty->sigDoEvent(onEvent, id, loopIdx, pMenu);
            });
        }
        return pSprite;
    }
    else
    {
        return oxygine::spActor();
    }
}

void NetworkGameDataView::close()
{
    detach();
}

void NetworkGameDataView::getMinimapImage(QImage & img, NetworkGameData & data)
{
    QBuffer buffer(&data.getMinimapData());
    buffer.open(QIODevice::ReadOnly);
    img.load(&buffer, "PNG");
}

QString NetworkGameDataView::getMapName() const
{
    return m_data.getMapName();
}

QString NetworkGameDataView::getDescription() const
{
    return m_data.getDescription();
}

qint32 NetworkGameDataView::getPlayers() const
{
    return m_data.getPlayers();
}

qint32 NetworkGameDataView::getMaxPlayers() const
{
    return m_data.getMaxPlayers();
}

QString NetworkGameDataView::getPlayerName(qint32 player)
{
    auto & info = m_data.getPlayerNames();
    if (player >= 0 && player < info.size())
    {
        return info[player];
    }
    return tr("Unknown");
}

bool NetworkGameDataView::getPlayerIsOnline(qint32 player)
{
    auto info = m_data.getOnlineData();
    if (player >= 0 && player < info.size())
    {
        return info[player];
    }
    return false;
}

qint32 NetworkGameDataView::getModCount() const
{
    return m_data.getMods().size();
}

QString NetworkGameDataView::getModName(qint32 mod) const
{
    auto & info = m_data.getMods();
    if (mod >= 0 && mod < info.size())
    {
        return Settings::getInstance()->getModName(info[mod]);
    }
    return tr("Unknown");
}

qint32 NetworkGameDataView::getObservers() const
{
    return m_data.getObservers();
}

qint32 NetworkGameDataView::getMaxObservers() const
{
    return m_data.getMaxObservers();
}
