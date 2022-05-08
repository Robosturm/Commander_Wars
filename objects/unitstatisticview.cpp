#include "objects/unitstatisticview.h"

#include "objects/base/label.h"

#include "resource_management/unitspritemanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "wiki/unitinfo.h"
#include "wiki/wikidatabase.h"

#include "game/player.h"
#include "game/gamemap.h"

UnitStatisticView::UnitStatisticView(const GameRecorder::PlayerData & data, qint32 width, qint32 heigth, Player* pPlayer, GameMap* pMap)
{
    setObjectName("UnitStatisticView");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    QSize size(width, heigth);
    spPanel pPanel = spPanel::create(true, size, size);
    addChild(pPanel);
    qint32 y = 10;
    addStatistic(pPanel, tr("Produced"), data.producedUnits, pPlayer, pMap, y, width);
    addStatistic(pPanel, tr("Lost"), data.lostUnits, pPlayer, pMap, y, width);
    addStatistic(pPanel, tr("Destroyed"), data.killedUnits, pPlayer, pMap, y, width);
    pPanel->setContentHeigth(y + 40);
    connect(this, &UnitStatisticView::sigShowLink, this, &UnitStatisticView::showLink, Qt::QueuedConnection);
}

void UnitStatisticView::showLink(QString pageID)
{
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    oxygine::Stage::getStage()->addChild(pWikiDatabase->getPage(pWikiDatabase->getEntry(pageID)));
}

void UnitStatisticView::addStatistic(spPanel & pPanel, QString headline, const QVector<GameRecorder::UnitData> & view, Player* pPlayer, GameMap* pMap, qint32 & y, qint32 width)
{
    qint32 x = 10;
    qint32 textWidth = 100;
    oxygine::TextStyle styleHeadline = oxygine::TextStyle(FontManager::getMainFont48());
    styleHeadline.color = FontManager::getFontColor();
    styleHeadline.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    styleHeadline.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    styleHeadline.multiline = false;
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    spLabel headlineLabel = spLabel::create(width - 50);
    headlineLabel->setStyle(styleHeadline);
    headlineLabel->setHtmlText(headline);
    headlineLabel->setPosition(x, y);
    pPanel->addItem(headlineLabel);
    y += 70;

    qint32 count = pMap->getPlayerCount();
    for (qint32 player = -1; player < count; ++player)
    {
        bool added = false;
        for (auto & item : view)
        {
            if (item.player == player)
            {
                added = true;
                QString unitId = item.unitId;
                Player* pUnitPlayer = pPlayer;
                if (player >= 0)
                {
                    pUnitPlayer = pMap->getPlayer(player);
                }
                spUnit pDummy = spUnit::create(item.unitId, pUnitPlayer, false, pMap);
                pDummy->setPosition(x, y);
                pDummy->addClickListener([this, unitId](oxygine::Event*)
                {
                    emit sigShowLink(unitId);
                });
                pPanel->addItem(pDummy);
                spLabel pLabel = spLabel::create(textWidth);
                pLabel->setStyle(style);
                pLabel->setHtmlText("x " + QString::number(item.count));
                pLabel->setPosition(x + 30, y - 5);
                pPanel->addItem(pLabel);
                x += 40 + textWidth;
                if (x + 50 + textWidth > width && player != count - 1)
                {
                    x = 10;
                    y += 40;
                }
            }
        }
        if (player != count - 1 && added)
        {
            x = 10;
            y += 40;
        }
    }
    y += 40;
}
