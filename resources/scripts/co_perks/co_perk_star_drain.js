var Constructor = function()
{
	this.gainPowerstar = function(co, fundsDamage, x, y, hpDamage, defender, counterAttack)
    {
		if (CO_PERK.isActive(co))
		{
            var powerGain = CO.getStarGain(co, fundsDamage, x, y, hpDamage, defender, counterAttack);
            co.setPowerFilled(co.getPowerFilled() - powerGain * 0.2);
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Decreases the power charge speed by 20%.");
    };
    this.getIcon = function()
    {
        return "star_drain";
    };
    this.getName = function()
    {
        return qsTr("Star Drain -20%");
    };
    this.getGroup = function()
    {
        return qsTr("Power Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_STAR_DRAIN = new Constructor();
