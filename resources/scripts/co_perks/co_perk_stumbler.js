var Constructor = function()
{
    this.getMovementpointModifier = function(co, unit, posX, posY, map)
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
        return qsTr("Decreases the movement range of all units by 1 during power and by 2 during superpower.");
    };
    this.getIcon = function(map)
    {
        return "stumbler";
    };
    this.getName = function()
    {
        return qsTr("Stumbler");
    };
    this.getGroup = function()
    {
        return qsTr("Power Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_STUMBLER = new Constructor();
