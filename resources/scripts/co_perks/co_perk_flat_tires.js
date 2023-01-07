var Constructor = function()
{
    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
		if (CO_PERK.isActive(co))
		{
			if (unit.isTransporter())
			{
				return -1;
			}
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Decreases the movement range of all transporters by 1.");
    };
    this.getIcon = function(map)
    {
        return "flat_tires";
    };
    this.getName = function()
    {
        return qsTr("Flat Tires -1");
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
var CO_PERK_FLAT_TIRES = new Constructor();
