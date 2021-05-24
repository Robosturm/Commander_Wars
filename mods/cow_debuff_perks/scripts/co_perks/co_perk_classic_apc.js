var Constructor = function()
{
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (defender.isTransporter())
			{
				return -20;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the transporter defense by 20%.";
    };
    this.getIcon = function()
    {
        return "classic_apc";
    };
    this.getName = function()
    {
        return "Classic APC -20%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_CLASSIC_APC = new Constructor();
