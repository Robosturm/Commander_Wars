var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Flare");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return -1;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_FLARE = new Constructor();
