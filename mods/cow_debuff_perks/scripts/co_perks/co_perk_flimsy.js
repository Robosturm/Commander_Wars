var Constructor = function()
{
        this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			return -10;
		}
        return 0;
    };
	this.gainPowerstar = function(co, fundsDamage, x, y, hpDamage, defender, counterAttack)
    {
		if (CO_PERK.isActive(co))
		{
            var powerGain = CO.getStarGain(co, fundsDamage, x, y, hpDamage, defender, counterAttack);
            co.setPowerFilled(co.getPowerFilled() + powerGain * 0.1);
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Decreases the defence of units by 10% and increases the power charge speed by 10%.";
    };
    this.getIcon = function()
    {
        return "flimsy";
    };
    this.getName = function()
    {
        return "Flimsy -10%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_FLIMSY = new Constructor();
