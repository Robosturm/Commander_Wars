var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (typeof map !== 'undefined')
			{
				if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                {
					return -20;
				}
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the attack of units by 20% during sandstorm.";
    };
    this.getIcon = function()
    {
        return "sandy_eyes";
    };
    this.getName = function()
    {
        return "Sandy Eyes";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SANDY_EYES = new Constructor();
