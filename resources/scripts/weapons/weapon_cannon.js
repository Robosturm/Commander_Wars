 Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Cannon");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 45;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// infantry
			case "INFANTRY":
				return WEAPON.HighDamage;
			case "MECH":
				return WEAPON.HighDamage;
			case "MOTORBIKE":
				return WEAPON.HighDamage;
			case "SNIPER":
				return WEAPON.HighDamage;

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
            case "HOVERTANK":
                return WEAPON.softDamage;
				
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
                return WEAPON.softDamage;
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
				return 100;
			case "CRUISER":
				return WEAPON.mediumDamage;
			case "DESTROYER":
				return WEAPON.mediumDamage;
			case "SUBMARINE":
				return WEAPON.mediumDamage;
			case "LANDER":
				return WEAPON.mediumDamage;
			case "AIRCRAFTCARRIER":
				return WEAPON.mediumDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
 WEAPON_CANNON = new Constructor();
