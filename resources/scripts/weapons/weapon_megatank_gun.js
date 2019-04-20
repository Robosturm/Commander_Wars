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
                return WEAPON.HighDamage;
            case "HOVERTANK":
                return WEAPON.HighDamage;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 75;
			case "HEAVY_TANK":
                return 75;
			case "NEOTANK":
                return 75;
				
			// very heavy tanks
			case "MEGATANK":
				return WEAPON.mediumDamage;
				
			// ranged land units
			case "ARTILLERY":
                return 105;
			case "ANTITANKCANNON":
				return 40;
			case "MISSILE":
                return 105;
            case "ROCKETTHROWER":
                return 105;
			
			// ships
			case "BATTLESHIP":
				return 20;
			case "CANNONBOAT":
				return 65;
			case "CRUISER":
				return 20;
			case "DESTROYER":
				return 20;
			case "SUBMARINE":
				return 20;
			case "LANDER":
				return 20;
			case "AIRCRAFTCARRIER":
				return 20;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MEGATANK_GUN = new Constructor();
