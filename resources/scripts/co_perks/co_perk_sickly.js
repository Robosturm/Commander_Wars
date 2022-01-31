var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
		if (CO_PERK.isActive(co))
		{
			if (map !== null)
			{
				if (map.onMap(atkPosX, atkPosY))
				{
					var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
                    var isForest = 	(terrainID === "FOREST") ||
                                    (terrainID === "FOREST1") ||
                                    (terrainID === "FOREST2") ||
                                    (terrainID === "FOREST3") ||
									(terrainID === "DESERT_FOREST") ||
                                    (terrainID === "DESERT_FOREST1") ||
                                    (terrainID === "SNOW_FOREST") ||
                                    (terrainID === "SNOW_FOREST1") ||
                                    (terrainID === "SNOW_FOREST2");
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
        return qsTr("Decreases the attack from woods by 10%.");
    };
    this.getIcon = function(map)
    {
        return "sickly";
    };
    this.getName = function()
    {
        return qsTr("Sickly");
    };
    this.getGroup = function()
    {
        return qsTr("Terrain Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SICKLY = new Constructor();
