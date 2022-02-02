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
                    var isSea = 	(terrainID === "SEA") ||
                                    (terrainID === "LAKE") ||
									(terrainID === "REAF") ||
									(terrainID === "ROUGH_SEA") ||
									(terrainID === "FOG");
                    if (isSea)
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
        return qsTr("Decreases the attack from sea by 10%.");
    };
    this.getIcon = function(map)
    {
        return "seasick";
    };
    this.getName = function()
    {
        return qsTr("Seasick");
    };
    this.getGroup = function()
    {
        return qsTr("Terrain Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SEASICK = new Constructor();
