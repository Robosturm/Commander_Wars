#include "wiki/fieldinfo.h"
#include "wiki/terraininfo.h"
#include "wiki/unitinfo.h"

FieldInfo::FieldInfo(Terrain* pTerrain, Unit* pUnit)
{
    setObjectName("FieldInfo");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);

    qint32 y = 10;
    if (pTerrain != nullptr)
    {
        spTerrainInfo pTerrainInfo = spTerrainInfo::create(pTerrain, m_pPanel->getWidth() - 80);
        pTerrainInfo->setPosition(20, y);
        m_pPanel->addItem(pTerrainInfo);
        m_pPanel->setContentHeigth(pTerrainInfo->getY() + pTerrainInfo->getHeight());
        y = pTerrainInfo->getY() + pTerrainInfo->getHeight() + 10;
    }
    if (pUnit != nullptr)
    {
        spUnitInfo pUnitInfo = spUnitInfo::create(pUnit, m_pPanel->getWidth() - 80);
        pUnitInfo->setPosition(20, y);
        m_pPanel->addItem(pUnitInfo);
        m_pPanel->setContentHeigth(pUnitInfo->getY() + pUnitInfo->getHeight());
    }

}

