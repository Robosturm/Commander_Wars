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
				return 75;
			case "MECH":
				return 75;
			case "MOTORBIKE":
				return 75;
			case "SNIPER":
				return 75;

			// supporter
			case "APC":
				return WEAPON.softCounterDamage;
			case "FLARE":
				return WEAPON.softCounterDamage;
			case "RECON":
				return WEAPON.softCounterDamage;

			// tanks
			case "FLAK":
				return 1;
			case "HOVERFLAK":
				return 1;
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
				return WEAPON.HighDamage;
			case "K_HELI":
				return WEAPON.softDamage;
				
			// ranged land units
			case "ARTILLERY":
				return 25;
			case "ANTITANKCANNON":
                return 1;
			case "MISSILE":
                return 25;
            case "ROCKETTHROWER":
				return 35;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_K_HELI_MG = new Constructor();
