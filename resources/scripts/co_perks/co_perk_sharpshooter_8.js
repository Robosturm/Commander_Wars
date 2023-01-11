var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
		if (CO_PERK.isActive(co))
		{
			if (attacker.getBaseMaxRange() > 1)
			{
				return 8;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the indirect attack of units by 8%.");
    };
    this.getIcon = function(map)
    {
        return "sniper";
    };
    this.getName = function()
    {
        return qsTr("Sharpshooter +8%");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive");
    };    
    this.getCosts = function()
    {
        return 2;
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SHARPSHOOTER_8 = new Constructor();
