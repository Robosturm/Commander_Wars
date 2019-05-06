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
                return 5;
            case "CANNONBOAT":
                return 25;
				
            case "CRUISER":
                return 25;
            case "DESTROYER":
                return 25;
            case "SUBMARINE":
                return 90;
				
            case "LANDER":
                return 25;
            case "BLACK_BOAT":
                return 25;
            case "AIRCRAFTCARRIER":
                return 5;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_CRUISER_MISSILE = new Constructor();
