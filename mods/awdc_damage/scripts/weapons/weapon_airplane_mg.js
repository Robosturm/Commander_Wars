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
                return 45;

			// supporter
			case "APC":
                return 15;
			case "FLARE":
                return 15;
			case "RECON":
                return 18;

			// tanks
			case "FLAK":
                return 5;
			case "HOVERFLAK":
				return 5;
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
				
			// ranged land units
			case "ARTILLERY":
                return 15;
			case "ANTITANKCANNON":
                return 5;
			case "MISSILE":
                return 20;
            case "ROCKETTHROWER":
				return 20;
				
			// air
			case "DUSTER":
                return 55;
			case "FIGHTER":
                return 40;
			case "BOMBER":
                return 45;
			case "STEALTHBOMBER":
                return 45;
			case "TRANSPORTPLANE":
                return 65;
            case "BLACK_BOMB":
                return 65;
			case "WATERPLANE":
                return 45;
			case "K_HELI":
				return 85;
			case "T_HELI":
				return 95;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_AIRPLANE_MG = new Constructor();
