var Constructor = function()
{
    this.getMovementcostModifier = function(co, unit, posX, posY)
    {
		if (CO_PERK.isActive(co))
		{
			if (typeof map !== 'undefined')
			{
				if (map.onMap(posX, posY))
				{
					var terrainID = map.getTerrain(posX, posY).getID();
					var isSea = 	(terrainID === "SEA") ||
									(terrainID === "REAF") ||
									(terrainID === "ROUGH_SEA") ||
									(terrainID === "FOG");
                    if (isSea)
                    {
                        return -999;
                    }
				}
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Sets movement costs on sea to 1.";
    };
    this.getIcon = function()
    {
        return "seamanship";
    };
    this.getName = function()
    {
        return "Seawalker";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SEAWALKER = new Constructor();
