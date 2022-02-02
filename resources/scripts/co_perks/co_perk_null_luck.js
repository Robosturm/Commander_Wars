var Constructor = function()
{
    this.getBonusLuck = function(co, unit, posX, posY, map)
    {
		if (CO_PERK.isActive(co))
		{
			return -10;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Decreases the maximum luck of units by 10%.");
    };
    this.getIcon = function(map)
    {
        return "null_luck";
    };
    this.getName = function()
    {
        return qsTr("Null Luck -10%");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_NULL_LUCK = new Constructor();
