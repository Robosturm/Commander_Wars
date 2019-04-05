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
                return 80;
            case "CANNONBOAT":
                return 120;
				
            case "CRUISER":
                return 20;
            case "DESTROYER":
                return 55;
            case "SUBMARINE":
                return 55;
				
            case "LANDER":
                return 85;
            case "AIRCRAFTCARRIER":
                return 110;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_TORPEDO = new Constructor();
