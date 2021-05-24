var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (attacker.isStatusStealthed())
			{
				return -20;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the dive/hide attack by 20%.";
    };
    this.getIcon = function()
    {
        return "facestab";
    };
    this.getName = function()
    {
        return "Facestab";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_FACESTAB = new Constructor();
