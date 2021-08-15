var Constructor = function()
{
    this.getBonusIncome = function(co, building, income)
    {
		if (CO_PERK.isActive(co))
		{
			return -income * 0.1;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Decreases the income from each building by 10%.");
    };
    this.getIcon = function()
    {
        return "income_tax";
    };
    this.getName = function()
    {
        return qsTr("Income Tax");
    };
    this.getGroup = function()
    {
        return qsTr("General Debuff");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_INCOME_TAX = new Constructor();
