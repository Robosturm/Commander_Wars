var Constructor = function()
{
    this.getBonusLuck = function(co, unit, posX, posY, map)
    {
		if (CO_PERK.isActive(co))
		{
			return 10;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the luck of units by 10%.");
    };
    this.getIcon = function(map)
    {
        return "luck";
    };
    this.getName = function()
    {
        return qsTr("Luck +10%");
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
var CO_PERK_LUCK_10 = new Constructor();
