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
					var isStreet = 	(terrainID === "STREET") ||
                                    (terrainID === "STREET1") ||
                                    (terrainID === "SNOW_STREET") ||
									(terrainID === "BRIDGE") ||
                                    (terrainID === "BRIDGE1") ||
                                    (terrainID === "WASTE_PATH") ||
                                    (terrainID === "DESERT_PATH") ||
                                    (terrainID === "DESERT_PATH1");
					if (isStreet)
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
        return qsTr("Increases the attack from roads by 10%.");
    };
    this.getIcon = function(map)
    {
        return "roadrage";
    };
    this.getName = function()
    {
        return qsTr("Road Rage");
    };
    this.getGroup = function()
    {
        return qsTr("Terrain");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_ROADRAGE = new Constructor();
