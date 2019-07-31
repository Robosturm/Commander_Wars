#include "terraininfo.h"

#include "resource_management/fontmanager.h"

#include "resource_management/movementtablemanager.h"

TerrainInfo::TerrainInfo(Terrain* pTerrain, qint32 width)
    : QObject()
{
    setWidth(width);

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    QString name = pTerrain->getTerrainName();
    QString description = pTerrain->getDescription();
    Building* pBuilding = pTerrain->getBuilding();
    if (pBuilding != nullptr)
    {
        name = pBuilding->getName();
        description = pBuilding->getDescription();
    }
    // no the fun begins create checkboxes and stuff and a panel down here
    qint32 y = 0;
    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText((tr("Terrain Information ") + name).toStdString().c_str());
    pLabel->setScale(2.0f);
    pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth(), 0);
    addChild(pLabel);
    y += 60;

    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(description.toStdString().c_str());
    pLabel->setScale(1.0f);
    pLabel->setPosition(0, y);
    addChild(pLabel);
    y += 40;

    pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Movement Costs").toStdString().c_str());
    pLabel->setScale(2.0f);
    pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth(), y);
    addChild(pLabel);
    y += 60;

    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    qint32 x = 0;
    for (qint32 i = 0; i < pMovementTableManager->getMovementTableCount(); i++)
    {
        QString name = pMovementTableManager->getMovementName(pMovementTableManager->getMovementID(i));
        qint32 costs = pMovementTableManager->getBaseMovementPoints(pMovementTableManager->getMovementID(i), pTerrain);

        pLabel = new oxygine::TextField();
        pLabel->setWidth(width);
        pLabel->setStyle(style);
        pLabel->setHtmlText((name + ":").toStdString().c_str());
        pLabel->setScale(1.0f);
        pLabel->setPosition(x, y);
        addChild(pLabel);
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width);
        pLabel->setStyle(style);
        if (costs > 0)
        {
            pLabel->setHtmlText(QString::number(costs).toStdString().c_str());
        }
        else
        {
            pLabel->setHtmlText("-");
        }
        pLabel->setScale(1.0f);
        pLabel->setPosition(x + 120, y);
        addChild(pLabel);
        x += 170;
        if (x + 210 > width && i < pMovementTableManager->getMovementTableCount() - 1)
        {
            x = 0;
            y += 40;
        }
    }
    y += 40;
    setHeight(y);
}
