var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Sniper Rifle");
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
				return 150;
			case "MECH":
				return 150;
			case "MOTORBIKE":
				return 150;
			case "SNIPER":
				return 150;

			// supporter
			case "APC":
				return WEAPON.lowDamage;
			case "FLARE":
				return WEAPON.lowDamage;
			case "RECON":
				return WEAPON.lowDamage;

			// tanks
			case "FLAK":
				return 8;
			case "HOVERFLAK":
				return 8;
			case "LIGHT_TANK":
				return 8;
            case "HOVERTANK":
				return 8;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
				return 5;
			case "HEAVY_TANK":
				return 5;
			case "NEOTANK":
				return 5;
				
			// very heavy tanks
			case "MEGATANK":
				return 1;
				
			// heli copter
			case "T_HELI":
				return WEAPON.softCounterDamage;
			case "K_HELI":
				return WEAPON.lowDamage;
				
			// ranged land units
			case "ARTILLERY":
				return 8;
			case "ANTITANKCANNON":
                return 8;
			case "MISSILE":
                return 12;
            case "ROCKETTHROWER":
				return 12;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_SNIPER = new Constructor();
