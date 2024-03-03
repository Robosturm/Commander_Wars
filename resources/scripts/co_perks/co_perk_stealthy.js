var Constructor = function()
{
    this.getFuelCostModifier = function(co, unit, x, y, costs, map)
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
        return qsTr("Dive/Hide fuel cost -2.");
    };
    this.getIcon = function(map)
    {
        return "stealthy";
    };
    this.getName = function()
    {
        return qsTr("Stealthy");
    };
    this.getCosts = function()
    {
        return 2;
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_STEALTHY = new Constructor();
