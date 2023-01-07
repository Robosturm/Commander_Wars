var Constructor = function()
{
	this.getBonusLuck = function(co, unit, posX, posY, map)
    {
		if (CO_PERK.isActive(co))
		{
			switch (co.getPowerMode())
			{
				case GameEnums.PowerMode_Tagpower:
				case GameEnums.PowerMode_Superpower:
					return 45;
				case GameEnums.PowerMode_Power:
					return 30;
				default:
					return 15;
			}
		}
        return 0;
    };
	this.getBonusMisfortune = function(co, unit, posX, posY, map)
    {
        if (CO_PERK.isActive(co))
		{
			switch (co.getPowerMode())
			{
				case GameEnums.PowerMode_Tagpower:
				case GameEnums.PowerMode_Superpower:
					return 45;
				case GameEnums.PowerMode_Power:
					return 30;
				default:
					return 15;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("+15%/+10% Max/Min Luck Roll, 2x Effect during COP, 3x Effect during SCOP");
    };
    this.getIcon = function(map)
    {
        return "enflakenator";
    };
    this.getName = function()
    {
        return qsTr("Enflakenator -10%");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive Debuff");
    };
    this.getCosts = function()
    {
        return -2;
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_ENFLAKENATOR = new Constructor();
