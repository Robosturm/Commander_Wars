var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
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
				return 120;
			case "MECH":
				return 120;
			case "MOTORBIKE":
				return 120;
			case "SNIPER":
				return 120;
				
			// supporter
			case "APC":
				return 80;
			case "FLARE":
				return 80;
			case "RECON":
				return 80;

			// tanks
			case "FLAK":
                return 75;
			case "HOVERFLAK":
                return 75;
			case "LIGHT_TANK":
                return 75;
            case "HOVERCRAFT":
                return 75;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 55;
			case "HEAVY_TANK":
                return 55;
			case "NEOTANK":
                return 55;
				
			// very heavy tanks
			case "MEGATANK":
				return 45;

            case "HOELLIUM":
                return 15;
				
			// ranged land units
			case "ARTILLERY":
                return 80;
			case "ANTITANKCANNON":
                return 65;
			case "MISSILE":
                return 85;
            case "ROCKETTHROWER":
                return 85;
            case "PIPERUNNER":
                return 85;
			
			// ships
			case "BATTLESHIP":
                return 55;
			case "CANNONBOAT":
				return 105;
			case "CRUISER":
                return 55;
			case "DESTROYER":
                return 55;
			case "SUBMARINE":
                return 65;
            case "LANDER":
                return 75;
            case "BLACK_BOAT":
                return 105;
			case "AIRCRAFTCARRIER":
                return 55;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ROCKET_MISSILE = new Constructor();
