var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// infantry
			case "INFANTRY":
				return 120;
			case "MECH":
				return 120;
			case "MOTORBIKE":
				return 120;
			case "SNIPER":
				return 120;
				
			// supporter
			case "APC":
				return 80;
			case "FLARE":
				return 80;
			case "RECON":
				return 80;

			// tanks
			case "FLAK":
                return 75;
			case "HOVERFLAK":
                return 75;
			case "LIGHT_TANK":
                return 75;
            case "HOVERTANK":
                return 75;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return WEAPON.mediumDamage;
			case "HEAVY_TANK":
                return WEAPON.mediumDamage;
			case "NEOTANK":
                return WEAPON.mediumDamage;
				
			// very heavy tanks
			case "MEGATANK":
				return 45;
				
			// ranged land units
			case "ARTILLERY":
                return WEAPON.HighDamage;
			case "ANITANKCANNON":
				return WEAPON.softDamage;
			case "MISSILE":
                return WEAPON.HighDamage;
            case "ROCKETTHROWER":
                return WEAPON.HighDamage;
			
			// ships
			case "BATTLESHIP":
				return WEAPON.mediumDamage;
			case "CANNONBOAT":
				return 105;
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
var WEAPON_ROCKET_MISSILE = new Constructor();
