var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Ship Missile");
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
            // ships
            case "BATTLESHIP":
                return WEAPON.softCounterDamage;
            case "CANNONBOAT":
                return 85;
				
            case "CRUISER":
                return WEAPON.softCounterDamage;
            case "DESTROYER":
                return WEAPON.lowDamage;
            case "SUBMARINE":
                return WEAPON.HighDamage;
				
            case "LANDER":
                return WEAPON.softDamage;
            case "AIRCRAFTCARRIER":
                return WEAPON.mediumDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_CRUISER_MISSILE = new Constructor();
