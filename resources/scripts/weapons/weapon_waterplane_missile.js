var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
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
				return WEAPON.HighDamage;
			case "FLARE":
				return WEAPON.HighDamage;
			case "RECON":
				return WEAPON.HighDamage;

			// tanks
			case "FLAK":
                return WEAPON.softCounterDamage;
			case "HOVERFLAK":
                return WEAPON.softCounterDamage;
			case "LIGHT_TANK":
                return 75;
			case "HOVERTANK":
                return 75;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
				return WEAPON.softDamage;
			case "HEAVY_TANK":
				return WEAPON.softDamage;
			case "NEOTANK":
				return WEAPON.softDamage;
				
			// very heavy tanks
			case "MEGATANK":
                return WEAPON.mediumDamage;
				
			// ranged land units
			case "ARTILLERY":
                return WEAPON.softDamage;
			case "ANTITANKCANNON":
				return WEAPON.mediumDamage;
			case "MISSILE":
				return WEAPON.softDamage;
            case "ROCKETTHROWER":
				return WEAPON.softDamage;				
			
			case "DUSTER":
				return WEAPON.mediumDamage;
			case "FIGHTER":
				return WEAPON.mediumDamage;
			case "BOMBER":
				return WEAPON.mediumDamage;
			case "STEALTHBOMBER":
				return WEAPON.mediumDamage;
			case "TRANSPORTPLANE":
				return WEAPON.mediumDamage;
			case "WATERPLANE":
				return WEAPON.mediumDamage;
			case "K_HELI":
				return WEAPON.HighDamage;
			case "T_HELI":
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
				return WEAPON.softDamage;
			case "AIRCRAFTCARRIER":
                return WEAPON.mediumDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_WATERPLANE_MISSILE = new Constructor();
