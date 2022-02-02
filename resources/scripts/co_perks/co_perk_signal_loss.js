var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
		if (CO_PERK.isActive(co))
		{
			var towers = co.getOwner().getBuildingCount("TOWER");        
			return -towers * 5;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Decreases the attack boost per tower by 5%.");
    };
    this.getIcon = function(map)
    {
        return "signal_loss";
    };
    this.getName = function()
    {
        return qsTr("Signal Loss");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SIGNAL_LOSS = new Constructor();
