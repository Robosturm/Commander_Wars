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
                return 65;
            case "CANNONBOAT":
                return 95;
				
            case "CRUISER":
                return 25;
            case "DESTROYER":
                return 55;
            case "SUBMARINE":
                return 55;
				
            case "LANDER":
                return 95;
            case "BLACK_BOAT":
                return 95;
            case "AIRCRAFTCARRIER":
                return 75;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_TORPEDO = new Constructor();
