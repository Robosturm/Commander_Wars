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
				return WEAPON.lowDamage;
			case "MOTORBIKE":
				return WEAPON.HighDamage;
			case "SNIPER":
				return WEAPON.HighDamage;

			// supporter
			case "APC":
				return 10;
			case "FLARE":
				return 10;
			case "RECON":
				return 10;

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
				return 30;
			case "K_HELI":
				return 10;
				
			// ranged land units
			case "ARTILLERY":
				return 10;
			case "ANITANKCANNON":
                return 45;
			case "MISSILE":
                return 20;
            case "ROCKETTHROWER":
				return 20;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_INFANTRY_MG = new Constructor();
