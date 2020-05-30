var Constructor = function()
{
    this.gainPowerstar = function(co, fundsDamage, x, y)
    {
		if (CO_PERK.isActive(co))
		{
			co.setPowerFilled(co.getPowerFilled() + (fundsDamage / 11000) * 1.2)
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Power meter fills up more quickly.";
    };
    this.getIcon = function()
    {
        return "starpower";
    };
    this.getName = function()
    {
        return "Star Power";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_STAR_POWER = new Constructor();
