var Constructor = function()
{
    this.getCaptureBonus = function(co, unit, posX, posY)
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
        return "Increases the capture rate by 1.";
    };
    this.getIcon = function()
    {
        return "invader";
    };
    this.getName = function()
    {
        return "Invader";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_INVADER = new Constructor();
