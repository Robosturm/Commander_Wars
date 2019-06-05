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
                return 95;
			case "MECH":
                return 90;
			case "MOTORBIKE":
                return 90;
			case "SNIPER":
                return 95;
				
			// supporter
			case "APC":
				return 80;
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

            case "HOELLIUM":
                return 15;
				
			// ranged land units
			case "ARTILLERY":
                return 80;
			case "ANTITANKCANNON":
                return 65;
			case "MISSILE":
                return 90;
            case "ROCKETTHROWER":
                return 85;
            case "PIPERUNNER":
                return 80;
			
			// ships
			case "BATTLESHIP":
                return 55;
			case "CANNONBOAT":
                return 60;
			case "CRUISER":
                return 60;
			case "DESTROYER":
                return 60;
			case "SUBMARINE":
                return 85;
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
var WEAPON_ROCKET_MISSILE = new Constructor();
