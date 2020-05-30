var Constructor = function()
{
    this.getCOUnitRange = function(co)
    {
		if (CO_PERK.isActive(co))
		{
			return 1;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Increases the CO-Zone of the CO by 1.";
    };
    this.getIcon = function()
    {
        return "co0";
    };
    this.getName = function()
    {
        return "Leader";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_LEADER = new Constructor();
