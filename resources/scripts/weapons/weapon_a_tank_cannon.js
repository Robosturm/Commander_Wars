var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Anti Tank Cannon");
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
				return WEAPON.lowDamage;
			case "MECH":
				return WEAPON.lowDamage;
			case "MOTORBIKE":
				return WEAPON.lowDamage;
			case "SNIPER":
				return WEAPON.lowDamage;

			// supporter
			case "APC":
				return WEAPON.HighDamage;
			case "FLARE":
				return WEAPON.HighDamage;
			case "RECON":
				return WEAPON.HighDamage;

			// tanks
			case "FLAK":
				return WEAPON.HighDamage;
			case "HOVERFLAK":
				return WEAPON.HighDamage;
			case "LIGHT_TANK":
				return WEAPON.HighDamage;
            case "HOVERCRAFT":
				return WEAPON.HighDamage;
				
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
				
			// heli copter
			case "T_HELI":
				return WEAPON.mediumDamage;
			case "K_HELI":
				return 45;
				
			// ranged land units
			case "ARTILLERY":
                return WEAPON.softDamage;
            case "ANTITANKCANNON":
				return WEAPON.mediumDamage;
			case "MISSILE":
                return WEAPON.softDamage;
            case "ROCKETTHROWER":
                return WEAPON.softDamage;
			
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_A_TANK_CANNON = new Constructor();
