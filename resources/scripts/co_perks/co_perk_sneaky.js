var Constructor = function()
{
    this.getFuelCostModifier = function(co, unit, costs, map)
    {
		if (CO_PERK.isActive(co))
		{
			if (unit.getHidden())
			{
				return -1;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Dive/Hide fuel cost -1.");
    };
    this.getIcon = function(map)
    {
        return "sneaky";
    };
    this.getName = function()
    {
        return qsTr("Sneaky");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SNEAKY = new Constructor();
