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
				return 75;
			case "MOTORBIKE":
				return 75;
			case "SNIPER":
				return 75;

			// supporter
			case "APC":
				return 75;
			case "FLARE":
				return 75;
			case "RECON":
				return 75;

			// tanks
			case "FLAK":
                return WEAPON.softDamage;
			case "HOVERFLAK":
                return WEAPON.softDamage;
			case "LIGHT_TANK":
                return WEAPON.softDamage;
            case "HOVERCRAFT":
                return WEAPON.softDamage;
				
			// heavy tanks
            case "HEAVY_HOVERCRAFT":
                return WEAPON.mediumDamage;
			case "HEAVY_TANK":
                return WEAPON.mediumDamage;
			case "NEOTANK":
                return WEAPON.mediumDamage;
				
			// very heavy tanks
			case "MEGATANK":
                return 40;
				
			// ranged land units
			case "ARTILLERY":
                return 70;
            case "ANTITANKCANNON":
                return WEAPON.mediumDamage;
			case "MISSILE":
                return WEAPON.softDamage;
            case "ROCKETTHROWER":
                return WEAPON.softDamage;

            // ships
            case "BATTLESHIP":
                return WEAPON.mediumDamage;
            case "CANNONBOAT":
                return 95;
            case "CRUISER":
                return WEAPON.mediumDamage;
            case "DESTROYER":
                return WEAPON.mediumDamage;
            case "SUBMARINE":
                return WEAPON.mediumDamage;
            case "LANDER":
                return 75;
            case "AIRCRAFTCARRIER":
                return WEAPON.mediumDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_BATTLESHIP_CANNON = new Constructor();
