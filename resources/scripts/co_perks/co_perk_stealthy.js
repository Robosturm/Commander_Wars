var Constructor = function()
{
    this.getFuelCostModifier = function(co, unit, costs)
    {
		if (CO_PERK.isActive(co))
		{
			if (unit.getHidden())
			{
				return -2;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Dive/Hide fuel cost -2.";
    };
    this.getIcon = function()
    {
        return "sneaky";
    };
    this.getName = function()
    {
        return "Stealthy";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_STEALTHY = new Constructor();
