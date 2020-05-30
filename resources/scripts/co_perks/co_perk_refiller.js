var Constructor = function()
{
	this.activatePower = function(co)
    {
		CO_PERK_REFILLER.refill(co);
    };

    this.activateSuperpower = function(co, powerMode)
    {
		CO_PERK_REFILLER.refill(co);
    };
	this.refill = function(co)
	{
		if (CO_PERK.isActive(co))
		{
			var units = co.getOwner().getUnits();
			units.randomize();
			for (var i = 0; i < units.size(); i++)
			{
				var unit = units.at(i);
				unit.refill();
			}
			units.remove();
		}
	};
	// Perk - Intel
    this.getDescription = function()
    {
        return "Refills ammo and fuel for all your units during Power and Superpower.";
    };
    this.getIcon = function()
    {
        return "ration";
    };
    this.getName = function()
    {
        return "Refiller";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_REFILLER = new Constructor();
