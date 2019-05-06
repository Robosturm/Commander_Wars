var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 25;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			case "APC":
                return 60;
			case "FLARE":
                return 55;
			case "RECON":
                return 55;

			case "FLAK":
                return 25;
			case "HOVERFLAK":
                return 25;
			case "LIGHT_TANK":
                return 55;
            case "HOVERCRAFT":
                return 55;

            case "HEAVY_HOVERCRAFT":
                return 25;
			case "HEAVY_TANK":
                return 25;
			case "NEOTANK":
                return 20;

			case "MEGATANK":
                return 10;

			case "ARTILLERY":
                return 65;
            case "ANTITANKCANNON":
                return 35;
			case "MISSILE":
                return 65;
            case "ROCKETTHROWER":
                return 65;

            case "HOELLIUM":
                return 25;
				
			// ships
			case "BATTLESHIP":
				return 25;
			case "CANNONBOAT":
                return 25;
			case "CRUISER":
                return 25;
			case "DESTROYER":
                return 25;
			case "SUBMARINE":
				return 25;
			case "LANDER":
				return 25;
            case "BLACK_BOAT":
                return 25;
			case "AIRCRAFTCARRIER":
				return 25;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_AIR_TO_GROUND_MISSILE = new Constructor();
