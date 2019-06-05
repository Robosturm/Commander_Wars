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
				return 130;
			case "MECH":
				return 130;
			case "MOTORBIKE":
				return 130;
			case "SNIPER":
				return 130;

			// supporter
			case "APC":
				return 50;
			case "FLARE":
				return 50;
			case "RECON":
				return 50;

			// tanks
			case "FLAK":
				return 10;
			case "HOVERFLAK":
				return 10;
			case "LIGHT_TANK":
				return 10;
            case "HOVERCRAFT":
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

            case "HOELLIUM":
                return 20;
				
			// heli copter
			case "T_HELI":
				return 45;
			case "K_HELI":
				return 35;
				
			// ranged land units
			case "ARTILLERY":
                return 45;
			case "ANTITANKCANNON":
                return 1;
			case "MISSILE":
                return 65;
            case "ROCKETTHROWER":
                return 65;
            case "PIPERUNNER":
                return 10;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MEGATANK_MG = new Constructor();
