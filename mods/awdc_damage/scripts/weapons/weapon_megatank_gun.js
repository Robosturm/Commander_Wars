var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 65;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// supporter
			case "APC":
				return 105;
			case "FLARE":
				return 105;
			case "RECON":
				return 105;

			// tanks
			case "FLAK":
                return 105;
			case "HOVERFLAK":
                return 105;
			case "LIGHT_TANK":
                return 85;
            case "HOVERCRAFT":
                return 85;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 75;
			case "HEAVY_TANK":
                return 75;
			case "NEOTANK":
                return 75;
				
			// very heavy tanks
			case "MEGATANK":
                return 55;

            case "HOELLIUM":
                return 35;
				
			// ranged land units
			case "ARTILLERY":
                return 105;
			case "ANTITANKCANNON":
                return 45;
			case "MISSILE":
                return 105;
            case "ROCKETTHROWER":
                return 105;
			
			// ships
			case "BATTLESHIP":
                return 12;
			case "CANNONBOAT":
				return 65;
            case "BLACK_BOAT":
                return 65;
			case "CRUISER":
                return 14;
			case "DESTROYER":
                return 14;
			case "SUBMARINE":
                return 14;
			case "LANDER":
                return 28;
			case "AIRCRAFTCARRIER":
                return 12;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MEGATANK_GUN = new Constructor();
