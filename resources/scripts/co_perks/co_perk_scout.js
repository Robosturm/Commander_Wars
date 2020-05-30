var Constructor = function()
{
    this.getVisionrangeModifier = function(co, unit, posX, posY)
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
        return "Increases the vision range of units by 1.";
    };
    this.getIcon = function()
    {
        return "eagleeye";
    };
    this.getName = function()
    {
        return "Scout";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SCOUT = new Constructor();
