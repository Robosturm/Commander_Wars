var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (attacker.getBaseMaxRange() > 1)
			{
				return 5;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the indirect attack of units by 5%.");
    };
    this.getIcon = function()
    {
        return "sharpshooter";
    };
    this.getName = function()
    {
        return qsTr("Sharpshooter +5%");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SHARPSHOOTER_5 = new Constructor();
