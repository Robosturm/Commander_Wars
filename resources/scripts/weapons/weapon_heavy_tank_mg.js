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
				return WEAPON.HighDamage;
			case "MECH":
				return WEAPON.HighDamage;
			case "MOTORBIKE":
				return WEAPON.HighDamage;
			case "SNIPER":
				return WEAPON.HighDamage;

			// supporter
			case "APC":
				return 45;
			case "FLARE":
				return 45;
			case "RECON":
				return 45;

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
				return 40;
			case "K_HELI":
				return 25;
				
			// ranged land units
			case "ARTILLERY":
				return 45;
			case "ANTITANKCANNON":
                return 1;
			case "MISSILE":
                return WEAPON.softDamage;
			case "ROCKETTRHOWER":
				return WEAPON.softDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_HEAVY_TANK_MG = new Constructor();
