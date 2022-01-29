var Constructor = function()
{
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, map)
    {
		if (CO_PERK.isActive(co))
		{
			if (defender.isTransporter())
			{
				return 10;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the transporter defense by 10%.");
    };
    this.getIcon = function(map)
    {
        return "apcguard";
    };
    this.getName = function()
    {
        return qsTr("Defense +10%");
    };
    this.getGroup = function()
    {
        return qsTr("Defensive");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_APC_GUARD = new Constructor();
