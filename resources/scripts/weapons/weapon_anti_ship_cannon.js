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
                return 95;
            case "CANNONBOAT":
                return 120;
				
            case "CRUISER":
                return 75;
            case "DESTROYER":
                return 55;
            case "SUBMARINE":
                return 65;
				
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
