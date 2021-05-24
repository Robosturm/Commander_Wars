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
					var isStreet = 	(terrainID === "STREET") ||
									(terrainID === "BRIDGE") ||
									(terrainID === "DESERT_PATH");
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
        return "Increases the attack from roads by 10%.";
    };
    this.getIcon = function()
    {
        return "roadrage";
    };
    this.getName = function()
    {
        return "Road Rage";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_ROADRAGE = new Constructor();
