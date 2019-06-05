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
				return 75;
			case "MECH":
                return 70;
			case "MOTORBIKE":
                return 70;
			case "SNIPER":
                return 70;

			// supporter
			case "APC":
                return 65;
			case "FLARE":
                return 70;
			case "RECON":
                return 70;

			// tanks
			case "FLAK":
                return 65;
			case "HOVERFLAK":
                return 65;
			case "LIGHT_TANK":
                return 65;
            case "HOVERCRAFT":
                return 65;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 50;
			case "HEAVY_TANK":
                return 50;
			case "NEOTANK":
                return 50;
				
			// very heavy tanks
			case "MEGATANK":
                return 40;

            case "HOELLIUM":
                return 20;
				
			// ranged land units
			case "ARTILLERY":
                return 70;
			case "ANTITANKCANNON":
                return 55;
			case "MISSILE":
                return 75;
            case "ROCKETTHROWER":
                return 75;

            case "PIPERUNNER":
                return 75;

            // ships
            case "BATTLESHIP":
                return 45;
            case "CANNONBOAT":
                return 95;
            case "BLACK_BOAT":
                return 95;
            case "CRUISER":
                return 65;
            case "DESTROYER":
                return 65;
            case "SUBMARINE":
                return 65;
            case "LANDER":
                return 75;
            case "AIRCRAFTCARRIER":
                return 50;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_BATTLESHIP_CANNON = new Constructor();
