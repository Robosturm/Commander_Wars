#ifndef BOXTERRAINPALETTEPREVIEW_H
#define BOXTERRAINPALETTEPREVIEW_H

#include "objects/base/box9object.h"
#include "game/terrain.h"

class BoxTerrainPalettePreview;
using spBoxTerrainPalettePreview = std::shared_ptr<BoxTerrainPalettePreview>;

class BoxTerrainPalettePreview : public Box9Object
{
    Q_OBJECT
public:
    BoxTerrainPalettePreview();

    Q_INVOKABLE void applyPalette(const QString & palette);
    Q_INVOKABLE void addTerrain(const QString & id, qint32 x, qint32 y);
private:
    QVector<spTerrain> m_terrains;
};

Q_DECLARE_INTERFACE(BoxTerrainPalettePreview, "BoxTerrainPalettePreview");

#endif // BOXTERRAINPALETTEPREVIEW_H
