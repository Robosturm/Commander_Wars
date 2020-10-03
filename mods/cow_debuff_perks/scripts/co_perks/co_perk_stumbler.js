var Constructor = function()
{
    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
		if (CO_PERK.isActive(co))
		{
			switch (co.getPowerMode())
			{
				case GameEnums.PowerMode_Tagpower:
				case GameEnums.PowerMode_Superpower:
					return -2;
				case GameEnums.PowerMode_Power:
					return -1;
				default:
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "DEcreases the movement range of all units by 1 during power and by 2 during superpower.";
    };
    this.getIcon = function()
    {
        return "stumbler";
    };
    this.getName = function()
    {
        return "Stumbler";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_STUMBLER = new Constructor();
