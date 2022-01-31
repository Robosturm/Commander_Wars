var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
		if (CO_PERK.isActive(co))
		{
			if (attacker.getBaseMaxRange() === 1)
			{
				return 5;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the direct attack of units by 5%.");
    };
    this.getIcon = function(map)
    {
        return "brawler";
    };
    this.getName = function()
    {
        return qsTr("Bruiser +5%");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_BRUISER_5 = new Constructor();
