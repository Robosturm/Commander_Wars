var Constructor = function()
{
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			var rangedAttacked = (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1);
			if (!rangedAttacked)
			{
				return -10;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the direct-fire defense by 10%.";
    };
    this.getIcon = function()
    {
        return "glass_jaw";
    };
    this.getName = function()
    {
        return "Glass Jaw -10%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_GLASS_JAW = new Constructor();
