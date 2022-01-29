var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, map)
    {
		if (CO_PERK.isActive(co))
		{
			if (map !== null)
			{
				if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
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
        return qsTr("Increases the attack of units by 20% during snow.");
    };
    this.getIcon = function(map)
    {
        return "icebreaker";
    };
    this.getName = function()
    {
        return qsTr("Icebreaker");
    };
    this.getGroup = function()
    {
        return qsTr("Weather");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_ICEBREAKER = new Constructor();
