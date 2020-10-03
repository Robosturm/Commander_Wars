var Constructor = function()
{
    this.getCostModifier = function(co, id, baseCost)
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
        return "Production cost +10%.";
    };
    this.getIcon = function()
    {
        return "tax_hike";
    };
    this.getName = function()
    {
        return "Tax Hike 10%";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_TAX_HIKE = new Constructor();
