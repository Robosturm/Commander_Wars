var Constructor = function()
{
    this.getCostModifier = function(co, id, baseCost, posX, posY, map)
    {
		if (CO_PERK.isActive(co))
		{
			return -baseCost * 0.05;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Production cost -5%.");
    };
    this.getIcon = function(map)
    {
        return "saleprice";
    };
    this.getName = function()
    {
        return qsTr("Sale Price -5%");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_SALE_PRICE_5 = new Constructor();
