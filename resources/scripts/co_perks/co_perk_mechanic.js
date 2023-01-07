var Constructor = function()
{
    this.getRepairBonus = function(co, unit, posX, posY, map)
    {
		if (CO_PERK.isActive(co))
		{
			return 2;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the base repair by 2.");
    };
    this.getIcon = function(map)
    {
        return "mechanic";
    };
    this.getName = function()
    {
        return qsTr("Mechanic");
    };
    this.getCosts = function()
    {
        return 2;
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_MECHANIC = new Constructor();
