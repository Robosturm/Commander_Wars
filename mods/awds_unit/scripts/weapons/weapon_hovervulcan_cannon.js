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
                return 105;
			case "MECH":
                return 105;
			case "MOTORBIKE":
                return 105;
			case "SNIPER":
                return 105;

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
                return 25;
            case "HOVERCRAFT":
                return 25;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 10;
			case "HEAVY_TANK":
                return 10;
			case "NEOTANK":
                return 5;
				
			// very heavy tanks
			case "MEGATANK":
                return 1;

            case "HOELLIUM":
                return 30;
				
			// heli copter
			case "T_HELI":
                return 105;
			case "K_HELI":
				return 105;
				
			//air
			case "DUSTER":
                return 65;
			case "FIGHTER":
                return 65;
			case "BOMBER":
				return 75;
			case "STEALTHBOMBER":
				return 75;
            case "BLACK_BOMB":
                return 120;
			case "TRANSPORTPLANE":
                return 120;
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
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_HOVERVULCAN_CANNON = new Constructor();
