var Constructor = function()
{
    this.getBonusIncome = function(co, building, income, map)
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
        return qsTr("Increases the income from each building by 10%.");
    };
    this.getIcon = function(map)
    {
        return "goldrush";
    };
    this.getName = function()
    {
        return qsTr("Gold Rush");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_GOLD_RUSH = new Constructor();
