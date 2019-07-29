var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Vulcan Cannon");
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
                return 170;
			case "MECH":
				return 150;
			case "MOTORBIKE":
				return 150;
			case "SNIPER":
				return 150;

			// supporter
			case "APC":
                return 50;
			case "FLARE":
                return 45;
			case "RECON":
                return 60;

			// tanks
			case "FLAK":
                return 45;
			case "HOVERFLAK":
                return 45;
			case "LIGHT_TANK":
                return 15;
            case "HOVERCRAFT":
                return 15;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 10;
			case "HEAVY_TANK":
                return 10;
			case "NEOTANK":
                return 10;
				
			// very heavy tanks
			case "MEGATANK":
				return 5;

            case "HOELLIUM":
                return 30;
				
			// heli copter
			case "T_HELI":
				return 120;
			case "K_HELI":
				return 105;
				
			//air
			case "DUSTER":
				return 75;
			case "FIGHTER":
				return 75;
			case "BOMBER":
				return 75;
			case "STEALTHBOMBER":
				return 75;
            case "BLACK_BOMB":
                return 75;
			case "TRANSPORTPLANE":
				return 75;
			case "WATERPLANE":
				return 75;
				
			// ranged land units
			case "ARTILLERY":
                return 50;
			case "ANTITANKCANNON":
                return 25;
			case "MISSILE":
                return 55;
            case "ROCKETTHROWER":
                return 55;
            case "PIPERUNNER":
                return 25;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_HOVERVULCAN_CANNON = new Constructor();
