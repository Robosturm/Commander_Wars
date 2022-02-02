var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
		if (CO_PERK.isActive(co))
		{
			if (map !== null)
			{
				if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_RAIN")
                {
					return 20;
				}
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the attack of units by 20% during rain.");
    };
    this.getIcon = function(map)
    {
        return "highanddry";
    };
    this.getName = function()
    {
        return qsTr("High and Dry");
    };
    this.getGroup = function()
    {
        return qsTr("Weather");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_HIGH_AND_DRY = new Constructor();
