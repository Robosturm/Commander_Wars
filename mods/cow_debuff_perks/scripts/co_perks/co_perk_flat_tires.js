var Constructor = function()
{
    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
		if (CO_PERK.isActive(co))
		{
			if (unit.isTransporter())
			{
				return -1;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the movement range of all transporters by 1.";
    };
    this.getIcon = function()
    {
        return "flat_tires";
    };
    this.getName = function()
    {
        return "Flat Tires -1";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_FLAT_TIRES = new Constructor();
