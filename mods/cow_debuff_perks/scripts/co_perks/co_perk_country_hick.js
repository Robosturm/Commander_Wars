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
					var building = map.getTerrain(atkPosX, atkPosY).getBuilding();
					if (building !== null)
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
        return "Decreases the attack from buildings by 10%.";
    };
    this.getIcon = function()
    {
        return "country_hick";
    };
    this.getName = function()
    {
        return "Country Hick";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_COUNTRY_HICK = new Constructor();
