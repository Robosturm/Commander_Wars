#include "objects/boxterrainpalettepreview.h"

BoxTerrainPalettePreview::BoxTerrainPalettePreview()
{

}

void BoxTerrainPalettePreview::addTerrain(const QString & id, qint32 x, qint32 y)
{
    spTerrain pTerrain = Terrain::createTerrain(id, -1, -1, "", nullptr);
    pTerrain->setPosition(x, y);
    pTerrain->loadSprites();
    addChild(pTerrain);
    m_terrains.append(pTerrain);
}

void BoxTerrainPalettePreview::applyPalette(const QString & palette)
{
    for (auto & pTerrain : std::as_const(m_terrains))
    {
        pTerrain->setTerrainPalette(palette, true);
    }
}