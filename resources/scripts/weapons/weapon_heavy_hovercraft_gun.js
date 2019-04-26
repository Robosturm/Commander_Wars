var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 55;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// supporter
			case "APC":
				return 90;
			case "FLARE":
				return 90;
			case "RECON":
				return 95;

			// tanks
			case "FLAK":
                return 90;
			case "HOVERFLAK":
                return 90;
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
                return 35;
				
			// ranged land units
			case "ARTILLERY":
                return 85;
			case "ANTITANKCANNON":
                return 35;
			case "MISSILE":
                return 90;
            case "ROCKETTHROWER":
                return 90;
			
			// ships
			case "BATTLESHIP":
                return 10;
			case "CANNONBOAT":
                return 55;
			case "CRUISER":
                return 12;
			case "DESTROYER":
                return 12;
			case "SUBMARINE":
                return 12;
			case "LANDER":
                return 22;
            case "BLACK_BOAT":
                return 55;
			case "AIRCRAFTCARRIER":
                return 10;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_HEAVY_HOVERCRAFT_GUN = new Constructor();
