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
                return 38;
            case "CANNONBOAT":
                return 85;
            case "BLACK_BOAT":
                return 85;
				
            case "CRUISER":
                return 28;
            case "DESTROYER":
                return 28;
            case "SUBMARINE":
                return 95;
				
            case "LANDER":
                return 40;
            case "AIRCRAFTCARRIER":
                return 38;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_CRUISER_MISSILE = new Constructor();
