var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Cannon");
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
                return WEAPON.HighDamage;
            case "DESTROYER":
                return WEAPON.mediumDamage;
            case "SUBMARINE":
                return WEAPON.lowDamage;
				
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
var WEAPON_ANTI_SHIP_CANNON = new Constructor();
