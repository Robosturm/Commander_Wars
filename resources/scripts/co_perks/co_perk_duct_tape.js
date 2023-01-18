var Constructor = function()
{
    this.getRepairBonus = function(co, unit, posX, posY, map)
    {
		if (CO_PERK.isActive(co))
		{
			return -1;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Decreases the base repair by 1.");
    };
    this.getIcon = function(map)
    {
        return "duct_tape";
    };
    this.getName = function()
    {
        return qsTr("Duct Tape");
    };
    this.getGroup = function()
    {
        return qsTr("General Debuff");
    };
    this.getCosts = function()
    {
        return -1;
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_DUCT_TAPE = new Constructor();
