var Constructor = function()
{
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (defender.isTransporter())
			{
				return 10;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Increases the transporter defense by 10%.";
    };
    this.getIcon = function()
    {
        return "apcguard";
    };
    this.getName = function()
    {
        return "Defense +10%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_APC_GUARD = new Constructor();
