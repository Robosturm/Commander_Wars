var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 15;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// supporter
            case "APC":
                return 75;
            case "FLARE":
                return 80;
            case "RECON":
                return 85;

			// tanks
			case "FLAK":
                return 65;
			case "HOVERFLAK":
                return 65;
            case "LIGHT_TANK":
                return 55;
            case "HOVERCRAFT":
                return 55;
				
			// heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 15;
			case "HEAVY_TANK":
                return 15;
			case "NEOTANK":
                return 15;
				
			// very heavy tanks
			case "MEGATANK":
                return 10;

            case "HOELLIUM":
                return 20;
				
			// ranged land units
			case "ARTILLERY":
                return 70;
			case "ANTITANKCANNON":
                return 30;
			case "MISSILE":
                return 85;
            case "ROCKETTHROWER":
                return 85;
            case "PIPERUNNER":
                return 55;
			
			// ships
			case "BATTLESHIP":
                return 1;
			case "CANNONBOAT":
                return 10;
			case "CRUISER":
                return 5;
			case "DESTROYER":
                return 5;
			case "SUBMARINE":
                return 1;
			case "LANDER":
                return 10;
            case "BLACK_BOAT":
                return 10;
			case "AIRCRAFTCARRIER":
                return 1;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_HOVERCRAFT_GUN = new Constructor();
