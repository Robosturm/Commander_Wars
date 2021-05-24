var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (attacker.getBaseMaxRange() > 1)
			{
				return -10;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the indirect attack of units by 10%.";
    };
    this.getIcon = function()
    {
        return "glass_eyes";
    };
    this.getName = function()
    {
        return "Glass Eyes -10%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_GLASS_EYES = new Constructor();
