var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, map)
    {
		if (CO_PERK.isActive(co))
		{
			if (attacker.getBaseMaxRange() > 1)
			{
				return -10;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Decreases the indirect attack of units by 10%.");
    };
    this.getIcon = function(map)
    {
        return "glass_eyes";
    };
    this.getName = function()
    {
        return qsTr("Glass Eyes -10%");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_GLASS_EYES = new Constructor();
