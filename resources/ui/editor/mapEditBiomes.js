var MapEditBiomes =
{
    getSelectedX : function()
    {
        return currentMenu.getObject("StartX").getCurrentValue();
    },
    getSelectedWidth : function()
    {
        return currentMenu.getObject("Width").getCurrentValue();
    },
    getSelectedY : function()
    {
        return currentMenu.getObject("StartY").getCurrentValue();
    },
    getSelectedHeight : function()
    {
        return currentMenu.getObject("Height").getCurrentValue();
    },
    getSelectedNewPalette : function()
    {
        return currentMenu.getObject("NewPalette").getCurrentItem();
    },
    getSelectedNewBiome : function()
    {
        return currentMenu.getObject("NewBiome").getCurrentItem();
    },
    applyPalette : function()
    {
        let x = MapEditBiomes.getSelectedX();
        let y = MapEditBiomes.getSelectedY();
        let width = MapEditBiomes.getSelectedWidth();
        let height = MapEditBiomes.getSelectedHeight();
        let newPalette = MapEditBiomes.getSelectedNewPalette();
        let rect = Qt.rect(x, y, width, height);
        currentMenu.getBaseMenu().getMap().applyPaletteToArea(rect, newPalette);
    },
    applyBiome : function()
    {
        let x = MapEditBiomes.getSelectedX();
        let y = MapEditBiomes.getSelectedY();
        let width = MapEditBiomes.getSelectedWidth();
        let height = MapEditBiomes.getSelectedHeight();
        let newBiome = MapEditBiomes.getSelectedNewBiome();
        let rect = Qt.rect(x, y, width, height);
        currentMenu.getBaseMenu().getMap().applyBiomeToArea(rect, TERRAIN.terrainBiomeGroups[newBiome]);
    }

}
