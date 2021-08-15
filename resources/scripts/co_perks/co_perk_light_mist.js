var Constructor = function()
{
    this.getVisionrangeModifier = function(co, unit, posX, posY)
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
        return qsTr("Decreases the vision range of units by 1.");
    };
    this.getIcon = function()
    {
        return "light_mist";
    };
    this.getName = function()
    {
        return qsTr("Light Mist");
    };
    this.getGroup = function()
    {
        return qsTr("General Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_LIGHT_MIST = new Constructor();
