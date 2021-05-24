var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (attacker.getBaseMaxRange() === 1)
			{
				return -10;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the direct attack of units by 10%.";
    };
    this.getIcon = function()
    {
        return "softie";
    };
    this.getName = function()
    {
        return "Softie -10%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SOFTIE = new Constructor();
