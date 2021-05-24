var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (attacker.getBaseMaxRange() === 1)
			{
				return 5;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Increases the direct attack of units by 5%.";
    };
    this.getIcon = function()
    {
        return "brawler";
    };
    this.getName = function()
    {
        return "Bruiser +5%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_BRUISER_5 = new Constructor();
