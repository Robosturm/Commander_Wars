var Constructor = function()
{
	this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
		if (isDefender)
		{
			return -25;
		}
	}
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the counter attack by 25%.";
    };
    this.getIcon = function()
    {
        return "surrender";
    };
    this.getName = function()
    {
        return "Surrender -25%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SURRENDER = new Constructor();
