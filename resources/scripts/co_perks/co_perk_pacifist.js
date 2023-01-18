var Constructor = function()
{
    this.getCaptureBonus = function(co, unit, posX, posY, map)
    {
		if (CO_PERK.isActive(co))
		{
			return -2;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Decreases the capture rate by 2.");
    };
    this.getIcon = function(map)
    {
        return "pacifist";
    };
    this.getName = function()
    {
        return qsTr("Pacifist");
    };
    this.getGroup = function()
    {
        return qsTr("General Debuff");
    };
    this.getCosts = function()
    {
        return -2;
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_PACIFIST = new Constructor();
