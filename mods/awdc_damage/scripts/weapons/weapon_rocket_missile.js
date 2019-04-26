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
                return 90;
				
			// supporter
			case "APC":
				return 80;
			case "FLARE":
                return 85;
			case "RECON":
                return 90;

			// tanks
			case "FLAK":
                return 75;
			case "HOVERFLAK":
                return 75;
			case "LIGHT_TANK":
                return 70;
            case "HOVERCRAFT":
                return 70;
				
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
				
			// ranged land units
			case "ARTILLERY":
                return 80;
			case "ANTITANKCANNON":
                return 65;
			case "MISSILE":
                return 85;
            case "ROCKETTHROWER":
                return 85;
			
			// ships
			case "BATTLESHIP":
                return 55;
			case "CANNONBOAT":
				return 105;
            case "BLACK_BOAT":
                return 105;
			case "CRUISER":
                return 55;
			case "DESTROYER":
                return 55;
			case "SUBMARINE":
                return 65;
			case "LANDER":
                return 75;
			case "AIRCRAFTCARRIER":
                return 45;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ROCKET_MISSILE = new Constructor();
