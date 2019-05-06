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
                return 75;
			case "FLARE":
				return 85;
			case "RECON":
                return 85;

			// tanks
			case "FLAK":
                return 65;
			case "HOVERFLAK":
                return 65;
			case "LIGHT_TANK":
                return 55;
            case "HOVERCRAFT":
                return 55;
				
			// heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 15;
			case "HEAVY_TANK":
                return 15;
			case "NEOTANK":
                return 15;
				
			// very heavy tanks
			case "MEGATANK":
                return 5;

            case "HOELLIUM":
                return 30;
				
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
