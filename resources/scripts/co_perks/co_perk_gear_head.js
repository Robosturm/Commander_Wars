var Constructor = function()
{
    this.getRepairBonus = function(co, unit, posX, posY, map)
    {
		if (CO_PERK.isActive(co))
		{
			return 1;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the base repair by 1.");
    };
    this.getIcon = function(map)
    {
        return "mechanic";
    };
    this.getName = function()
    {
        return qsTr("Gear Head");
    };
    this.getCosts = function()
    {
        return 1;
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_GEAR_HEAD = new Constructor();
