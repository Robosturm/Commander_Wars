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
				return 8;
            case "HOVERCRAFT":
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
                return 85;
			case "K_HELI":
                return 65;
				
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
