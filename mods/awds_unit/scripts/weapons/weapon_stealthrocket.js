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
                return 90;
			case "MECH":
                return 90;
			case "MOTORBIKE":
                return 90;
			case "SNIPER":
                return 90;

			// supporter
			case "APC":
                return 85;
			case "FLARE":
                return 85;
			case "RECON":
                return 85;

			// tanks
			case "FLAK":
                return 50;
			case "HOVERFLAK":
                return 50;
			case "LIGHT_TANK":
                return 75;
			case "HOVERCRAFT":
                return 75;
				
			// heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 70;
			case "HEAVY_TANK":
                return 70;
			case "NEOTANK":
                return 60;
				
			// very heavy tanks
			case "MEGATANK":
                return 25;

            case "HOELLIUM":
                return 30;
				
			// ranged land units
			case "ARTILLERY":
                return 75;
			case "ANTITANKCANNON":
                return 55;
			case "MISSILE":
                return 85;
            case "ROCKETTHROWER":
                return 85;
            case "PIPERUNNER":
                return 80;

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
                return 75;
            case "K_HELI":
                return 115;
            case "T_HELI":
                return 115;


			// ships
			case "BATTLESHIP":
                return 45;
			case "CANNONBOAT":
                return 60;
			case "CRUISER":
                return 35;
			case "DESTROYER":
                return 35;
			case "SUBMARINE":
                return 55;
			case "LANDER":
                return 60;
            case "BLACK_BOAT":
                return 60;
			case "AIRCRAFTCARRIER":
                return 60;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_STEALTHROCKET = new Constructor();
