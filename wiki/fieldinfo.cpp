#include "wiki/fieldinfo.h"
#include "wiki/terraininfo.h"
#include "wiki/unitinfo.h"

FieldInfo::FieldInfo(spTerrain pTerrain, spUnit pUnit)
    : Wikipage(pUnit.get() != nullptr ? pUnit->getUnitID() : pTerrain->getID())
{
#ifdef GRAPHICSUPPORT
    setObjectName("FieldInfo");
#endif
    Interpreter::setCppOwnerShip(this);

    qint32 y = 10;
    if (pTerrain != nullptr)
    {
        spTerrainInfo pTerrainInfo = MemoryManagement::create<TerrainInfo>(pTerrain->getMap(), pTerrain.get(), m_pPanel->getScaledWidth() - 80);
        pTerrainInfo->setPosition(20, y);
        m_pPanel->addItem(pTerrainInfo);
        m_pPanel->setContentHeigth(pTerrainInfo->getY() + pTerrainInfo->getScaledHeight());
        y = pTerrainInfo->getY() + pTerrainInfo->getScaledHeight() + 10;
    }
    if (pUnit != nullptr)
    {
        spUnitInfo pUnitInfo = MemoryManagement::create<UnitInfo>(pUnit, m_pPanel->getScaledWidth() - 80);
        pUnitInfo->setPosition(20, y);
        m_pPanel->addItem(pUnitInfo);
        m_pPanel->setContentHeigth(pUnitInfo->getY() + pUnitInfo->getScaledHeight());
    }

}

