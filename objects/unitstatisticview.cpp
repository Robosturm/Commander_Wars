#include "objects/unitstatisticview.h"

#include "objects/base/label.h"

#include "resource_management/unitspritemanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "wiki/unitinfo.h"
#include "wiki/wikidatabase.h"

UnitStatisticView::UnitStatisticView(const GameRecorder::PlayerData & data, qint32 width, qint32 heigth, Player* pPlayer)
{
    setObjectName("UnitStatisticView");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    QSize size(width, heigth);
    spPanel pPanel = spPanel::create(true, size, size);
    addChild(pPanel);
    qint32 y = 10;
    addStatistic(pPanel, tr("Produced"), data.producedUnits, pPlayer, y, width);
    addStatistic(pPanel, tr("Lost"), data.lostUnits, pPlayer, y, width);
    addStatistic(pPanel, tr("Destroyed"), data.killedUnits, pPlayer, y, width);
    pPanel->setContentHeigth(y + 40);
    connect(this, &UnitStatisticView::sigShowLink, this, &UnitStatisticView::showLink, Qt::QueuedConnection);
}

void UnitStatisticView::showLink(QString pageID)
{
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    oxygine::getStage()->addChild(pWikiDatabase->getPage(pWikiDatabase->getEntry(pageID)));
}

void UnitStatisticView::addStatistic(spPanel & pPanel, QString headline, const QMap<QString, qint32> & view, Player* pPlayer, qint32 & y, qint32 width)
{
    qint32 x = 10;
    qint32 textWidth = 100;
    oxygine::TextStyle styleHeadline = FontManager::getMainFont48();
    styleHeadline.color = FontManager::getFontColor();
    styleHeadline.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    styleHeadline.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    styleHeadline.multiline = false;
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    auto iter = view.constBegin();
    spLabel headlineLabel = spLabel::create(width - 50);
    headlineLabel->setStyle(styleHeadline);
    headlineLabel->setHtmlText(headline);
    headlineLabel->setPosition(x, y);
    pPanel->addItem(headlineLabel);
    y += 70;
    while (iter != view.constEnd())
    {
        QString unitId = iter.key();
        spUnit pDummy = spUnit::create(unitId, pPlayer, false);
        pDummy->setPosition(x, y);
        pDummy->addClickListener([=](oxygine::Event*)
        {
            emit sigShowLink(unitId);
        });
        pPanel->addItem(pDummy);
        spLabel pLabel = spLabel::create(textWidth);
        pLabel->setStyle(style);
        pLabel->setHtmlText("x " + QString::number(iter.value()));
        pLabel->setPosition(x + 30, y - 5);
        pPanel->addItem(pLabel);
        x += 40 + textWidth;
        ++iter;
        if (x + 50 + textWidth > width && iter != view.constEnd())
        {
            x = 10;
            y += 40;
        }
    }
    y += 40;
}
