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
                return 40;
            case "CANNONBOAT":
                return 75;
            case "CRUISER":
                return 40;
            case "DESTROYER":
                return 40;
            case "SUBMARINE":
                return 40;
            case "LANDER":
                return 55;
            case "AIRCRAFTCARRIER":
                return 40;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_MISSILE = new Constructor();
