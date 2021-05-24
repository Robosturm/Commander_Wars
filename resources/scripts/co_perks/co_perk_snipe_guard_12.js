var Constructor = function()
{
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			var rangedAttacked = (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1);
			if (rangedAttacked)
			{
				return 12;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Increases the indirect-fire defense by 12%.";
    };
    this.getIcon = function()
    {
        return "snipeguard";
    };
    this.getName = function()
    {
        return "Snipe Guard +12%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SNIPE_GUARD_12 = new Constructor();
