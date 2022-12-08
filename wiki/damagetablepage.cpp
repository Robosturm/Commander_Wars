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
    const qint32 itemStart = 40;
    const QColor color(200, 200, 200, 50);
    const float scale = 0.5f;
    const qint32 itemStartCount = 2;
    const qint32 tableWidth = entryWidth * tableSize;
    const qint32 tableHeight = entryHeight * tableSize;
    const qint32 totalWidth = tableWidth * scale + itemStart * itemStartCount;
    const qint32 totalHeight = tableHeight * scale + itemStart * itemStartCount;


    oxygine::spActor pActor = oxygine::spActor::create();

    spLabel pText = spLabel::create(tableWidth + itemStart * itemStartCount);
    oxygine::TextStyle infoStyle(FontManager::getMainFont24());
    infoStyle.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    pText->setStyle(infoStyle);
    pText->setHtmlText(tr("Defender"));
    pText->setPosition(0, 0);
    pActor->addChild(pText);

    pText = spLabel::create(tableHeight + itemStart * itemStartCount);
    pText->setStyle(infoStyle);
    pText->setHtmlText(tr("Attacker"));
    pText->setPosition(0, tableHeight + itemStart * itemStartCount);
    pText->setRotationDegrees(-90);
    pActor->addChild(pText);

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
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getFont("damageTable20"));
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

    pText = spLabel::create(tableWidth + itemStart * itemStartCount);
    pText->setStyle(infoStyle);
    pText->setHtmlText(tr("Defender"));
    pText->setPosition(0, tableHeight + itemStart * itemStartCount);
    pActor->addChild(pText);

    pText = spLabel::create(tableHeight + itemStart * itemStartCount);
    pText->setStyle(infoStyle);
    pText->setHtmlText(tr("Attacker"));
    pText->setPosition(tableWidth + itemStart * itemStartCount, 0);
    pText->setRotationDegrees(90);
    pActor->addChild(pText);

    pActor->setScale(scale);
    m_pPanel->addItem(pActor);

    m_pPanel->setContentWidth(totalWidth);
    m_pPanel->setContentHeigth(totalHeight);
}
