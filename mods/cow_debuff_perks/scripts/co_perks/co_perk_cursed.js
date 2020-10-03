var Constructor = function()
{
	this.getBonusMisfortune = function(co, unit, posX, posY)
    {
        if (CO_PERK.isActive(co))
		{
			return 10;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Increases the missfortune of units by 10%.";
    };
    this.getIcon = function()
    {
        return "cursed";
    };
    this.getName = function()
    {
        return "Cursed -10%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_CURSED = new Constructor();
