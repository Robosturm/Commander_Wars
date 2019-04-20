var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Anti Tank Cannon");
    };

    this.getEnviromentDamage = function(enviromentId)
    {
        return 55;
    };

    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// infantry
			case "INFANTRY":
                return 75;
			case "MECH":
                return 65;
			case "MOTORBIKE":
                return 65;
			case "SNIPER":
                return 65;

			// supporter
			case "APC":
                return 65;
			case "FLARE":
                return 75;
			case "RECON":
                return 75;

			// tanks
			case "FLAK":
                return 75;
			case "HOVERFLAK":
                return 75;
			case "LIGHT_TANK":
                return 75;
            case "HOVERTANK":
                return 75;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 65;
			case "HEAVY_TANK":
                return 65;
			case "NEOTANK":
                return 65;
				
			// very heavy tanks
			case "MEGATANK":
                return 55;
				
			// heli copter
			case "T_HELI":
                return 45;
			case "K_HELI":
                return 55;
				
			// ranged land units
			case "ARTILLERY":
                return 65;
			case "ANTITANKCANNON":
                return 55;
			case "MISSILE":
                return 70;
            case "ROCKETTHROWER":
                return 70;
			
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_A_TANK_CANNON = new Constructor();
