var Constructor = function()
{
	this.activatePower = function(co, map)
    {
		CO_PERK_REFILLER.refill(co);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
		CO_PERK_REFILLER.refill(co);
    };
	this.refill = function(co)
	{
		if (CO_PERK.isActive(co))
		{
			var units = co.getOwner().getUnits();
			units.randomize();
            var size = units.size();
            for (var i = 0; i < size; i++)
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
        return qsTr("Refills ammo and fuel for all your units during Power and Superpower.");
    };
    this.getIcon = function(map)
    {
        return "ration";
    };
    this.getName = function()
    {
        return qsTr("Refiller");
    };
    this.getGroup = function()
    {
        return qsTr("Power");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_REFILLER = new Constructor();
