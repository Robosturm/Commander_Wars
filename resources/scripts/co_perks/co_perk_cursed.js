var Constructor = function()
{
	this.getBonusMisfortune = function(co, unit, posX, posY, map)
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
        return qsTr("Increases the missfortune of units by 10%.");
    };
    this.getIcon = function(map)
    {
        return "cursed";
    };
    this.getName = function()
    {
        return qsTr("Cursed -10%");
    };
    this.getGroup = function()
    {
        return qsTr("Offensive Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_CURSED = new Constructor();
