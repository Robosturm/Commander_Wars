var Constructor = function()
{
    this.getBonusIncome = function(co, building, income)
    {
		if (CO_PERK.isActive(co))
		{
			return income * 0.1;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return "Increases the income from each building by 10%.";
    };
    this.getIcon = function()
    {
        return "goldrush";
    };
    this.getName = function()
    {
        return "Gold Rush";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_GOLD_RUSH = new Constructor();
