var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
		if (CO_PERK.isActive(co))
		{
			if (attacker.isStatusStealthed())
			{
				return -20;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Decreases the dive/hide attack by 20%.");
    };
    this.getIcon = function()
    {
        return "facestab";
    };
    this.getName = function()
    {
        return qsTr("Facestab");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_FACESTAB = new Constructor();
