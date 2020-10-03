var Constructor = function()
{
    this.getBonusLuck = function(co, unit, posX, posY)
    {
		if (CO_PERK.isActive(co))
		{
			return -10;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the maximum luck of units by 10%.";
    };
    this.getIcon = function()
    {
        return "null_luck";
    };
    this.getName = function()
    {
        return "Null Luck -10%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_NULL_LUCK = new Constructor();
