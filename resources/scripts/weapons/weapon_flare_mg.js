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
                return 80;
			case "MECH":
                return 70;
			case "MOTORBIKE":
                return 70;
			case "SNIPER":
                return 70;

			// supporter
			case "APC":
				return 45;
			case "FLARE":
                return 50;
			case "RECON":
                return 60;

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
                return 35;
			case "K_HELI":
                return 18;
				
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
var WEAPON_FLARE_MG = new Constructor();
