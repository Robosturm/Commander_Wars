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
                                    (terrainID === "FOREST1") ||
                                    (terrainID === "FOREST2") ||
                                    (terrainID === "FOREST3") ||
									(terrainID === "DESERT_FOREST") ||
									(terrainID === "SNOW_FOREST");
                    if (isForest)
                    {
                        return 10;
                    }
				}
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the attack from woods by 10%.");
    };
    this.getIcon = function()
    {
        return "ranger";
    };
    this.getName = function()
    {
        return qsTr("Ranger");
    };
    this.getGroup = function()
    {
        return qsTr("Terrain");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_RANGER = new Constructor();
