var Constructor = function()
{
    this.getRepairBonus = function(co, unit, posX, posY)
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
    this.getIcon = function()
    {
        return "mechanic";
    };
    this.getName = function()
    {
        return qsTr("Mechanic");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_MECHANIC = new Constructor();
