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
                return 135;
			case "MECH":
                return 125;
			case "MOTORBIKE":
                return 125;
			case "SNIPER":
                return 135;

			// supporter
			case "APC":
                return 65;
			case "FLARE":
                return 65;
			case "RECON":
                return 65;

			// tanks
			case "FLAK":
                return 17;
			case "HOVERFLAK":
                return 17;
			case "LIGHT_TANK":
				return 10;
            case "HOVERCRAFT":
				return 10;
				
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
                return 55;
			case "K_HELI":
                return 22;
				
			// ranged land units
			case "ARTILLERY":
                return 65;
			case "ANTITANKCANNON":
                return 1;
			case "MISSILE":
                return 55;
            case "ROCKETTHROWER":
                return 75;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MEGATANK_MG = new Constructor();
