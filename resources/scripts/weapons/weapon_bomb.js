var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Bomb");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 95;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// infantry
			case "INFANTRY":
				return 115;
			case "MECH":
				return 110;
			case "MOTORBIKE":
				return 110;
			case "SNIPER":
				return 110;

			// supporter
			case "APC":
				return 105;
			case "FLARE":
				return 105;
			case "RECON":
				return 105;

			// tanks
			case "FLAK":
                return WEAPON.softCounterDamage;
			case "HOVERFLAK":
                return WEAPON.softCounterDamage;
			case "LIGHT_TANK":
                return WEAPON.softDamage;
			case "HOVERTANK":
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
                return WEAPON.softCounterDamage;
				
			// ranged land units
			case "ARTILLERY":
				return 75;
			case "ANITANKCANNON":
				return WEAPON.mediumDamage;
			case "MISSILE":
				return 75;
            case "ROCKETTHROWER":
				return 75;				
			
			// ships
			case "BATTLESHIP":
				return 85;
			case "CANNONBOAT":
				return 120;				
			case "CRUISER":
				return WEAPON.softCounterDamage;
			case "DESTROYER":
				return WEAPON.mediumDamage;
			case "SUBMARINE":
				return 85;				
			case "LANDER":
				return 95;
			case "AIRCRAFTCARRIER":
                return WEAPON.softDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_BOMB = new Constructor();
