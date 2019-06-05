var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 70;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// infantry
			case "INFANTRY":
                return 85;
			case "MECH":
                return 85;
			case "MOTORBIKE":
                return 85;
			case "SNIPER":
                return 85;

			// supporter
			case "APC":
                return 85;
			case "FLARE":
                return 85;
			case "RECON":
                return 85;

			// tanks
			case "FLAK":
                return 55;
			case "HOVERFLAK":
                return 55;
			case "LIGHT_TANK":
                return 55;
			case "HOVERCRAFT":
                return 55;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 35;
			case "HEAVY_TANK":
                return 35;
			case "NEOTANK":
                return 35;
				
			// very heavy tanks
			case "MEGATANK":
                return 15;

            case "HOELLIUM":
                return 30;
				
			// ranged land units
			case "ARTILLERY":
                return 65;
			case "ANTITANKCANNON":
                return 55;
			case "MISSILE":
                return 65;
            case "ROCKETTHROWER":
                return 65;
            case "PIPERUNNER":
                return 65;
			
            // air units
            case "DUSTER":
                return 65;
            case "FIGHTER":
                return 45;
            case "BOMBER":
                return 75;
            case "STEALTHBOMBER":
                return 75;
            case "TRANSPORTPLANE":
                return 120;
            case "BLACK_BOMB":
                return 120;
            case "WATERPLANE":
                return 55;
            case "K_HELI":
                return 85;
            case "T_HELI":
                return 95;

			// ships
			case "BATTLESHIP":
                return 55;
			case "CANNONBOAT":
                return 75;
            case "BLACK_BOAT":
                return 75;
			case "CRUISER":
                return 35;
			case "DESTROYER":
                return 35;
			case "SUBMARINE":
                return 55;
			case "LANDER":
                return 55;
			case "AIRCRAFTCARRIER":
                return 55;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_STEALTHROCKET = new Constructor();
