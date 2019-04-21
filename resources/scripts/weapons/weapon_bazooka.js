var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Bazooka");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 15;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// supporter
			case "APC":
				return 85;
			case "FLARE":
				return 85;
			case "RECON":
				return 75;

			// tanks
			case "FLAK":
                return 55;
			case "HOVERFLAK":
                return 55;
			case "LIGHT_TANK":
                return 55;
            case "HOVERCRAFT":
                return 55;
				
			// heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 25;
			case "HEAVY_TANK":
                return 25;
			case "NEOTANK":
                return 25;
				
			// very heavy tanks
			case "MEGATANK":
                return 15;
				
			// ranged land units
			case "ARTILLERY":
                return 70;
            case "ANTITANKCANNON":
                return 55;
			case "MISSILE":
                return 85;
            case "ROCKETTHROWER":
                return 85;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_BAZOOKA = new Constructor();
