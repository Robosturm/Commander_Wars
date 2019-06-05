var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Cannon");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 55;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// infantry
			case "INFANTRY":
                return 95;
			case "MECH":
                return 90;
			case "MOTORBIKE":
                return 90;
			case "SNIPER":
                return 90;

			// supporter
			case "APC":
                return 60;
			case "FLARE":
                return 90;
			case "RECON":
                return 90;

			// tanks
			case "FLAK":
                return 85;
			case "HOVERFLAK":
                return 85;
			case "LIGHT_TANK":
                return 80;
            case "HOVERCRAFT":
                return 80;
				
			// heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 55;
			case "HEAVY_TANK":
                return 55;
			case "NEOTANK":
                return 50;
				
			// very heavy tanks
			case "MEGATANK":
                return 25;
				
			// ranged land units
			case "ARTILLERY":
                return 80;
            case "ANTITANKCANNON":
                return 55;
			case "MISSILE":
                return 85;
            case "ROCKETTHROWER":
                return 85;
            case "PIPERUNNER":
                return 80;

            case "HOELLIUM":
                return 20;

            // ships
            case "BATTLESHIP":
                return 50;
            case "CANNONBOAT":
                return 95;
            case "CRUISER":
                return 95;
            case "DESTROYER":
                return 55;
            case "SUBMARINE":
                return 95;
            case "LANDER":
                return 95;
            case "BLACK_BOAT":
                return 95;
            case "AIRCRAFTCARRIER":
                return 60;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_BATTLESHIP_CANNON = new Constructor();
