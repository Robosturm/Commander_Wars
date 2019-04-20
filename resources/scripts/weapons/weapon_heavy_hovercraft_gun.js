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
                return 70;
			case "HOVERFLAK":
                return 70;
			case "LIGHT_TANK":
                return 70;
            case "HOVERTANK":
                return 70;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return WEAPON.mediumDamage;
			case "HEAVY_TANK":
                return 45;
			case "NEOTANK":
                return WEAPON.HighDamage;
				
			// very heavy tanks
			case "MEGATANK":
				return WEAPON.softCounterDamage;
				
			// ranged land units
			case "ARTILLERY":
                return WEAPON.HighDamage;
			case "ANTITANKCANNON":
				return WEAPON.softCounterDamage;
			case "MISSILE":
                return 95;
            case "ROCKETTHROWER":
                return 95;
			
			// ships
			case "BATTLESHIP":
				return WEAPON.lowDamage;
			case "CANNONBOAT":
				return WEAPON.mediumDamage;
			case "CRUISER":
				return WEAPON.lowDamage;
			case "DESTROYER":
				return WEAPON.lowDamage;
			case "SUBMARINE":
				return WEAPON.lowDamage;
			case "LANDER":
				return WEAPON.lowDamage;
			case "AIRCRAFTCARRIER":
				return WEAPON.lowDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_HEAVY_HOVERCRAFT_GUN = new Constructor();
