var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 15;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// supporter
			case "APC":
				return 85;
			case "FLARE":
				return 85;
			case "RECON":
				return 75;

			// tanks
			case "FLAK":
                return WEAPON.mediumDamage;
			case "HOVERFLAK":
                return WEAPON.mediumDamage;
			case "LIGHT_TANK":
                return WEAPON.mediumDamage;
            case "HOVERTANK":
                return WEAPON.HighDamage;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return WEAPON.softCounterDamage;
			case "HEAVY_TANK":
                return WEAPON.softCounterDamage;
			case "NEOTANK":
                return WEAPON.softCounterDamage;
				
			// very heavy tanks
			case "MEGATANK":
				return WEAPON.lowDamage;
				
			// ranged land units
			case "ARTILLERY":
                return 70;
			case "ANITANKCANNON":
				return WEAPON.lowDamage;
			case "MISSILE":
                return WEAPON.HighDamage;
            case "ROCKETTHROWER":
                return WEAPON.HighDamage;
			
			// ships
			case "BATTLESHIP":
				return 10;
			case "CANNONBOAT":
				return 45;
			case "CRUISER":
				return 10;
			case "DESTROYER":
				return 10;
			case "SUBMARINE":
				return 10;
			case "LANDER":
				return 10;
			case "AIRCRAFTCARRIER":
				return 10;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_LIGHT_TANK_GUN = new Constructor();
