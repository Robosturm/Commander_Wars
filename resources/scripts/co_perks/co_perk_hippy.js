var Constructor = function()
{
    this.getCaptureBonus = function(co, unit, posX, posY, map)
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
        return qsTr("Decreases the capture rate by 1.");
    };
    this.getIcon = function(map)
    {
        return "hippy";
    };
    this.getName = function()
    {
        return qsTr("Hippy");
    };
    this.getGroup = function()
    {
        return qsTr("General Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_HIPPY = new Constructor();
