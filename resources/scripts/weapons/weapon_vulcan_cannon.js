var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Vulcan Cannon");
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// infantry
			case "INFANTRY":
				return 150;
			case "MECH":
				return 150;
			case "MOTORBIKE":
				return 150;
			case "SNIPER":
				return 150;

			// supporter
			case "APC":
				return WEAPON.mediumDamage;
			case "FLARE":
				return WEAPON.mediumDamage;
			case "RECON":
				return WEAPON.mediumDamage;

			// tanks
			case "FLAK":
				return WEAPON.mediumDamage;
			case "HOVERFLAK":
				return WEAPON.mediumDamage;
			case "LIGHT_TANK":
				return WEAPON.lowDamage;
            case "HOVERTANK":
				return WEAPON.HighDamage;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
				return WEAPON.lowDamage;
			case "HEAVY_TANK":
				return WEAPON.lowDamage;
			case "NEOTANK":
				return WEAPON.lowDamage;
				
			// very heavy tanks
			case "MEGATANK":
				return 5;
				
			// heli copter
			case "T_HELI":
				return 120;
			case "K_HELI":
				return 105;
				
			//air
			case "DUSTER":
				return 75;
			case "FIGHTER":
				return 75;
			case "BOMBER":
				return 75;
			case "STEALTHBOMBER":
				return 75;
			case "TRANSPORTPLANE":
				return 75;
			case "WATERPLANE":
				return 75;
				
			// ranged land units
			case "ARTILLERY":
				return WEAPON.mediumDamage;
			case "ANITANKCANNON":
                return WEAPON.lowDamage;
			case "MISSILE":
                return WEAPON.mediumDamage;
			case "ROCKETTRHOWER":
				return WEAPON.mediumDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_VULCAN_CANNON = new Constructor();
