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
                return 105;
			case "MECH":
                return 85;
			case "MOTORBIKE":
                return 85;
			case "SNIPER":
                return 85;

			// supporter
			case "APC":
                return 75;
			case "FLARE":
                return 75;
			case "RECON":
                return 75;

			// tanks
			case "FLAK":
                return 35;
			case "HOVERFLAK":
                return 35;
			case "LIGHT_TANK":
                return 65;
			case "HOVERCRAFT":
                return 65;
				
			// heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 55;
			case "HEAVY_TANK":
                return 55;
			case "NEOTANK":
                return 55;
				
			// very heavy tanks
			case "MEGATANK":
                return 35;

            case "HOELLIUM":
                return 30;
				
			// ranged land units
			case "ARTILLERY":
                return 65;
			case "ANTITANKCANNON":
                return 55;
			case "MISSILE":
                return 55;
            case "ROCKETTHROWER":
                return 75;
            case "PIPERUNNER":
                return 75;

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
			case "CRUISER":
                return 35;
			case "DESTROYER":
                return 35;
			case "SUBMARINE":
                return 55;
			case "LANDER":
                return 55;
            case "BLACK_BOAT":
                return 75;
			case "AIRCRAFTCARRIER":
                return 55;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_STEALTHROCKET = new Constructor();
