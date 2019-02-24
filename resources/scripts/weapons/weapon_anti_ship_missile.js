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
                return 75;
            case "CRUISER":
                return WEAPON.softCounterDamage;
            case "DESTROYER":
                return WEAPON.softCounterDamage;
            case "SUBMARINE":
                return WEAPON.softCounterDamage;
            case "LANDER":
                return WEAPON.mediumDamage;
            case "AIRCRAFTCARRIER":
                return WEAPON.softCounterDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_MISSILE = new Constructor();
