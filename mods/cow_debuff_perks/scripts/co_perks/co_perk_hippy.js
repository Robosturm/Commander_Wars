var Constructor = function()
{
    this.getCaptureBonus = function(co, unit, posX, posY)
    {
		if (CO_PERK.isActive(co))
		{
			return -1;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the capture rate by 1.";
    };
    this.getIcon = function()
    {
        return "hippy";
    };
    this.getName = function()
    {
        return "Hippy";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_HIPPY = new Constructor();
