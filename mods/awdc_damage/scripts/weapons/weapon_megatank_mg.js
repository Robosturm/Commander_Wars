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
                return 105;
			case "MECH":
                return 95;
			case "MOTORBIKE":
                return 95;
			case "SNIPER":
                return 95;

			// supporter
			case "APC":
                return 45;
			case "FLARE":
                return 40;
			case "RECON":
                return 45;

			// tanks
			case "FLAK":
				return 10;
			case "HOVERFLAK":
				return 10;
			case "LIGHT_TANK":
				return 10;
            case "HOVERTANK":
				return 10;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
				return 10;
			case "HEAVY_TANK":
				return 10;
			case "NEOTANK":
				return 10;
				
			// very heavy tanks
			case "MEGATANK":
				return 1;
				
			// heli copter
			case "T_HELI":
				return 45;
			case "K_HELI":
				return 35;
				
			// ranged land units
			case "ARTILLERY":
                return 45;
			case "ANITANKCANNON":
                return 1;
			case "MISSILE":
                return 65;
            case "ROCKETTHROWER":
                return 65;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MEGATANK_MG = new Constructor();
