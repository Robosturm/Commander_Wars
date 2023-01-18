var Constructor = function()
{
    this.getCostModifier = function(co, id, baseCost, posX, posY, map)
    {
		if (CO_PERK.isActive(co))
		{
			return baseCost * 0.1;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Production cost +10%.");
    };
    this.getIcon = function(map)
    {
        return "tax_hike";
    };
    this.getName = function()
    {
        return qsTr("Tax Hike 10%");
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
var CO_PERK_TAX_HIKE = new Constructor();
