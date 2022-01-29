var Constructor = function()
{
	this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, map)
    {
		if (isDefender)
		{
			return -25;
		}
	}
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Decreases the counter attack by 25%.");
    };
    this.getIcon = function(map)
    {
        return "surrender";
    };
    this.getName = function()
    {
        return qsTr("Surrender -25%");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SURRENDER = new Constructor();
