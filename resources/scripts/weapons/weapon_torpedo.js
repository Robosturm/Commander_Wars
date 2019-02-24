var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Torpedo");
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
            // ships
            case "BATTLESHIP":
                return WEAPON.softDamage;
            case "CANNONBOAT":
                return 120;
				
            case "CRUISER":
                return WEAPON.lowDamage;
            case "DESTROYER":
                return WEAPON.HighDamage;
            case "SUBMARINE":
                return WEAPON.mediumDamage;
				
            case "LANDER":
                return 120;
            case "AIRCRAFTCARRIER":
                return 120;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_TORPEDO = new Constructor();
