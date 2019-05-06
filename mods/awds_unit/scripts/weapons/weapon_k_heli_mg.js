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
                return 20;
			case "FLARE":
                return 30;
			case "RECON":
                return 30;

			// tanks
			case "FLAK":
				return 1;
			case "HOVERFLAK":
				return 1;
			case "LIGHT_TANK":
                return 6;
            case "HOVERCRAFT":
                return 6;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 1;
			case "HEAVY_TANK":
                return 1;
			case "NEOTANK":
                return 1;
				
			// very heavy tanks
			case "MEGATANK":
				return 1;

            case "HOELLIUM":
                return 20;
				
			// heli copter
			case "T_HELI":
                return 95;
			case "K_HELI":
                return 65;
				
			// ranged land units
			case "ARTILLERY":
				return 25;
			case "ANTITANKCANNON":
                return 1;
			case "MISSILE":
                return 35;
            case "ROCKETTHROWER":
				return 35;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_K_HELI_MG = new Constructor();
