var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (typeof map !== 'undefined')
			{
				if (map.onMap(atkPosX, atkPosY))
				{
					var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
                    var isForest = 	(terrainID === "FOREST") ||
									(terrainID === "DESERT_FOREST") ||
									(terrainID === "SNOW_FOREST");
                    if (isForest)
                    {
                        return -10;
                    }
				}
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the attack from woods by 10%.";
    };
    this.getIcon = function()
    {
        return "sickly";
    };
    this.getName = function()
    {
        return "Sickly";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SICKLY = new Constructor();
