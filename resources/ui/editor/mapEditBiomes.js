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
        GameConsole.print(x.toString() + " " +
                          y.toString() + " " +
                          width.toString() + " " +
                          height.toString() + " " +
                          newPalette.toString(), 1);
    },
    applyBiome : function()
    {
        let x = MapEditBiomes.getSelectedX();
        let y = MapEditBiomes.getSelectedY();
        let width = MapEditBiomes.getSelectedWidth();
        let height = MapEditBiomes.getSelectedHeight();
        let newBiome = MapEditBiomes.getSelectedNewBiome();
        GameConsole.print(x.toString() + " " +
                          y.toString() + " " +
                          width.toString() + " " +
                          height.toString() + " " +
                          newBiome.toString(), 1);
    }

}
