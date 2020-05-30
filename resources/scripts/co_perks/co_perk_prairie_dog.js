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
					if (terrainID === "PLAINS")
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
        return "Sets movement costs over plains to 1.";
    };
    this.getIcon = function()
    {
        return "prairiedog";
    };
    this.getName = function()
    {
        return "Prairie Dog";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_PRAIRIE_DOG = new Constructor();
