var Constructor = function()
{
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
		if (CO_PERK.isActive(co))
		{
			var rangedAttacked = (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1);
			if (rangedAttacked)
			{
				return 8;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the indirect-fire defense by 8%.");
    };
    this.getIcon = function(map)
    {
        return "snipeguard";
    };
    this.getName = function()
    {
        return qsTr("Snipe Guard +8%");
    };
    this.getGroup = function()
    {
        return qsTr("Defensive");
    };
    this.getCosts = function()
    {
        return 1;
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SNIPE_GUARD_8 = new Constructor();
