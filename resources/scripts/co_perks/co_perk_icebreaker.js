var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (typeof map !== 'undefined')
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
        return "Increases the attack of units by 20% during snow.";
    };
    this.getIcon = function()
    {
        return "icebreaker";
    };
    this.getName = function()
    {
        return "Icebreaker";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_ICEBREAKER = new Constructor();
