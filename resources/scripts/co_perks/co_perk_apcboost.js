var Constructor = function()
{
    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        if (unit.isTransporter())
        {
            return 1;
        }
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Increases the movement range of all transporters by 1.";
    };
    this.getIcon = function()
    {
        return "apcboost";
    };
    this.getName = function()
    {
        return "Boost +1";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_APCBOOST = new Constructor();
