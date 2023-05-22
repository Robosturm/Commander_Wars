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
        var x = MapEditBiomes.getSelectedX();
        var y = MapEditBiomes.getSelectedY();
        var width = MapEditBiomes.getSelectedWidth();
        var height = MapEditBiomes.getSelectedHeight();
        var newPalette = MapEditBiomes.getSelectedNewPalette();
        var rect = Qt.rect(x, y, width, height);
        currentMenu.getBaseMenu().getMap().applyPaletteToArea(rect, newPalette);
    },
    applyBiome : function()
    {
        var x = MapEditBiomes.getSelectedX();
        var y = MapEditBiomes.getSelectedY();
        var width = MapEditBiomes.getSelectedWidth();
        var height = MapEditBiomes.getSelectedHeight();
        var newBiome = MapEditBiomes.getSelectedNewBiome();
        var rect = Qt.rect(x, y, width, height);
        currentMenu.getBaseMenu().getMap().applyBiomeToArea(rect, TERRAIN.terrainBiomeGroups[newBiome]);
    }

}
