var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 125;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// supporter
			case "APC":
                return 195;
			case "FLARE":
                return 195;
			case "RECON":
                return 195;

			// tanks
			case "FLAK":
                return 195;
			case "HOVERFLAK":
                return 195;
			case "LIGHT_TANK":
                return 180;
            case "HOVERCRAFT":
                return 180;
				
			// heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 125;
			case "HEAVY_TANK":
                return 125;
			case "NEOTANK":
                return 115;
				
			// very heavy tanks
			case "MEGATANK":
                return 65;
				
			// ranged land units
			case "ARTILLERY":
                return 195;
			case "ANTITANKCANNON":
                return 65;
			case "MISSILE":
                return 195;
            case "ROCKETTHROWER":
                return 195;

            case "HOELLIUM":
                return 35;
			
			// ships
			case "BATTLESHIP":
                return 45;
			case "CANNONBOAT":
                return 105;
			case "CRUISER":
                return 65;
			case "DESTROYER":
                return 45;
			case "SUBMARINE":
                return 45;
			case "LANDER":
                return 75;
            case "BLACK_BOAT":
                return 105;
			case "AIRCRAFTCARRIER":
                return 45;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MEGATANK_GUN = new Constructor();
