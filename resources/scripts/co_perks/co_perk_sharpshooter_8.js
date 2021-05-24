var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (attacker.getBaseMaxRange() > 1)
			{
				return 8;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Increases the indirect attack of units by 8%.";
    };
    this.getIcon = function()
    {
        return "sharpshooter";
    };
    this.getName = function()
    {
        return "Sharpshooter +8%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SHARPSHOOTER_8 = new Constructor();
