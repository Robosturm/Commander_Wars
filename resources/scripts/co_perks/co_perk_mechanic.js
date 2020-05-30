var Constructor = function()
{
    this.getRepairBonus = function(co, unit, posX, posY)
    {
		if (CO_PERK.isActive(co))
		{
			return 2;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Increases the base repair by 2.";
    };
    this.getIcon = function()
    {
        return "mechanic";
    };
    this.getName = function()
    {
        return "Gear Head";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_GEAR_HEAD = new Constructor();
