#include "wiki/damagetablepage.h"

#include "game/gamemap.h"

#include "resource_management/unitspritemanager.h"
#include "resource_management/fontmanager.h"

#include "objects/base/label.h"

DamageTablePage::DamageTablePage()
{
    const qint32 bonusWidth = 30;
    const qint32 entryWidth = GameMap::defaultImageSize + bonusWidth;
    const qint32 entryHeight = GameMap::defaultImageSize;
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    auto unitIds = pUnitSpriteManager->getUnitsSorted();
    const qint32 tableSize = unitIds.size() + 2;
    const qint32 itemStart = 10;
    const QColor color(200, 200, 200, 50);

    oxygine::spActor pActor = oxygine::spActor::create();

    auto pPlayer = spPlayer::create(nullptr);
    pPlayer->init();

    for (qint32 x = 1; x < tableSize; x +=2)
    {
        oxygine::spColorRectSprite pRect = oxygine::spColorRectSprite::create();
        pRect->setWidth(entryWidth * tableSize);
        pRect->setHeight(entryHeight);
        pRect->setColor(color);
        pRect->setPosition(itemStart, itemStart + x * entryHeight);
        pActor->addChild(pRect);
    }
    for (qint32 y = 1; y < tableSize; y +=2)
    {
        oxygine::spColorRectSprite pRect = oxygine::spColorRectSprite::create();
        pRect->setWidth(entryWidth);
        pRect->setHeight(entryHeight * tableSize);
        pRect->setColor(color);
        pRect->setPosition(itemStart + y * entryWidth, itemStart);
        pActor->addChild(pRect);
    }

    const QColor markerColor(0, 255, 0, 200);
    m_verticalMarker = oxygine::spColorRectSprite::create();
    m_verticalMarker->setWidth(entryWidth * tableSize);
    m_verticalMarker->setHeight(entryHeight);
    m_verticalMarker->setColor(markerColor);
    m_verticalMarker->setPosition(itemStart, itemStart + entryHeight);
    pActor->addChild(m_verticalMarker);

    m_horizontalMarker = oxygine::spColorRectSprite::create();
    m_horizontalMarker->setWidth(entryWidth);
    m_horizontalMarker->setHeight(entryHeight * tableSize);
    m_horizontalMarker->setColor(markerColor);
    m_horizontalMarker->setPosition(itemStart + entryWidth, itemStart);
    pActor->addChild(m_horizontalMarker);

    QVector<spUnit> matchups;
    matchups.reserve(tableSize - 1);
    for (auto & unitId : unitIds)
    {
        spUnit pUnit = spUnit::create(unitId, pPlayer.get(), false, nullptr);
        matchups.append(pUnit);
    }
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = false;

    for (qint32 x = 1; x < tableSize - 1; ++x)
    {
        spUnit pUnitX = spUnit::create(unitIds[x - 1], pPlayer.get(), false, nullptr);
        pUnitX->setOwner(nullptr);
        pUnitX->setPosition(itemStart + x * entryWidth + bonusWidth  / 2,
                            itemStart);
        pActor->addChild(pUnitX);

        for (qint32 y = 1; y < tableSize - 1; ++y)
        {
            if (x == 1)
            {
                spUnit pUnitY = spUnit::create(unitIds[y - 1], pPlayer.get(), false, nullptr);
                pUnitY->setOwner(nullptr);
                pUnitY->setPosition(itemStart + bonusWidth  / 2,
                                    itemStart + y * entryHeight);
                pActor->addChild(pUnitY);

                spUnit pUnitY2 = spUnit::create(unitIds[y - 1], pPlayer.get(), false, nullptr);
                pUnitY2->setOwner(nullptr);
                pUnitY2->setPosition(itemStart + (tableSize - 1) * entryWidth +  bonusWidth  / 2,
                                    itemStart + y * entryHeight);
                pActor->addChild(pUnitY2);
            }
            spLabel pLabel = spLabel::create(entryWidth);
            qint32 posX = itemStart + x * entryWidth;
            qint32 posY = itemStart + y * entryHeight;
            pLabel->setPosition(posX, posY);
            pLabel->setStyle(style);
            qint32 damage = matchups[y - 1]->getBaseDamage(matchups[x - 1].get());
            if (damage >= 0)
            {
                pLabel->setHtmlText(QString::number(damage) + "%");
            }
            else
            {
                pLabel->setHtmlText("-");
            }
            pLabel->addEventListener(oxygine::TouchEvent::OVER, [this, posY, posX](oxygine::Event*)
            {
                m_verticalMarker->setY(posY);
                m_horizontalMarker->setX(posX);
            });
            pActor->addChild(pLabel);
        }

        spUnit pUnitX2 = spUnit::create(unitIds[x - 1], pPlayer.get(), false, nullptr);
        pUnitX2->setOwner(nullptr);
        pUnitX2->setPosition(itemStart + x * entryWidth + bonusWidth  / 2,
                             itemStart + (tableSize - 1) * entryHeight);
        pActor->addChild(pUnitX2);
    }

    const float scale = 0.5f;
    pActor->setScale(scale);
    m_pPanel->addItem(pActor);

    m_pPanel->setContentWidth(entryWidth * tableSize * scale + 50);
    m_pPanel->setContentHeigth(entryHeight * tableSize * scale + 50);
}
