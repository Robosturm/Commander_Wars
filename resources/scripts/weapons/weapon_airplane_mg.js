var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("MG");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 1;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// infantry
			case "INFANTRY":
				return WEAPON.mediumDamage;
			case "MECH":
				return WEAPON.mediumDamage;
			case "MOTORBIKE":
				return WEAPON.mediumDamage;
			case "SNIPER":
				return WEAPON.mediumDamage;

			// supporter
			case "APC":
				return WEAPON.lowDamage;
			case "FLARE":
				return WEAPON.lowDamage;
			case "RECON":
				return WEAPON.lowDamage;

			// tanks
			case "FLAK":
				return 5;
			case "HOVERFLAK":
				return 5;
			case "LIGHT_TANK":
				return 5;
            case "HOVERTANK":
				return 5;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
				return 3;
			case "HEAVY_TANK":
				return 3;
			case "NEOTANK":
				return 3;
				
			// very heavy tanks
			case "MEGATANK":
				return 1;
				
			// heli copter
			case "T_HELI":
				return 95;
			case "K_HELI":
				return 85;
				
			// ranged land units
			case "ARTILLERY":
				return WEAPON.lowDamage;
			case "ANITANKCANNON":
                return 5;
			case "MISSILE":
                return 20;
            case "ROCKETTHROWER":
				return 20;
				
			// air
			case "DUSTER":
				return WEAPON.mediumDamage;
			case "FIGHTER":
				return WEAPON.lowDamage;
			case "BOMBER":
				return WEAPON.mediumDamage;
			case "STEALTHBOMBER":
				return WEAPON.mediumDamage;
			case "TRANSPORTPLANE":
				return WEAPON.mediumDamage;
			case "WATERPLANE":
				return WEAPON.HighDamage;
			case "K_HELI":
				return 85;
			case "T_HELI":
				return 95;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_AIRPLANE_MG = new Constructor();
