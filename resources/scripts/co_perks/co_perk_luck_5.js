var Constructor = function()
{
    this.getBonusLuck = function(co, unit, posX, posY)
    {
		if (CO_PERK.isActive(co))
		{
			return 5;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the luck of units by 5%.");
    };
    this.getIcon = function()
    {
        return "luck";
    };
    this.getName = function()
    {
        return qsTr("Luck +5%");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_LUCK_5 = new Constructor();
