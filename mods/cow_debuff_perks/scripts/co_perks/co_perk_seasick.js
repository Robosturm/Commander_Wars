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
                    var isSea = 	(terrainID === "SEA") ||
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
        return "Decreases the attack from sea by 10%.";
    };
    this.getIcon = function()
    {
        return "seasick";
    };
    this.getName = function()
    {
        return "Seasick";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SEASICK = new Constructor();
