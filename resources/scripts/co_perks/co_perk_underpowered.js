var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
		if (CO_PERK.isActive(co))
		{
			return -10;
		}
        return 0;
    };
	this.gainPowerstar = function(co, fundsDamage, x, y, hpDamage, defender, counterAttack, map)
    {
		if (CO_PERK.isActive(co))
		{
            var powerGain = CO.getStarGain(co, fundsDamage, x, y, hpDamage, defender, counterAttack, map);
            co.setPowerFilled(co.getPowerFilled() + powerGain * 0.1);
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Decreases the attack of units by 10% and increases the power charge speed by 10%.");
    };
    this.getIcon = function(map)
    {
        return "underpowered";
    };
    this.getName = function()
    {
        return qsTr("Underpowered -10%");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_UNDERPOWERED = new Constructor();
