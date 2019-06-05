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
                return 55;
			case "MECH":
                return 45;
			case "MOTORBIKE":
                return 45;
			case "SNIPER":
                return 55;

			// supporter
			case "APC":
                return 14;
			case "FLARE":
				return 10;
			case "RECON":
                return 12;

			// tanks
			case "FLAK":
				return 5;
			case "HOVERFLAK":
				return 5;
			case "LIGHT_TANK":
				return 5;
            case "HOVERCRAFT":
				return 5;
				
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
				return 30;
			case "K_HELI":
                return 7;
				
			// ranged land units
			case "ARTILLERY":
                return 15;
			case "ANTITANKCANNON":
                return 45;
			case "MISSILE":
                return 25;
            case "ROCKETTHROWER":
                return 25;
            case "PIPERUNNER":
                return 5;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_INFANTRY_MG = new Constructor();
