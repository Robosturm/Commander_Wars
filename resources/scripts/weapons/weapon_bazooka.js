var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Bazooka");
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
                return WEAPON.mediumDamage;
			case "HOVERFLAK":
                return WEAPON.mediumDamage;
			case "LIGHT_TANK":
                return WEAPON.mediumDamage;
            case "HOVERTANK":
                return WEAPON.mediumDamage;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 25;
			case "HEAVY_TANK":
                return 25;
			case "NEOTANK":
                return 25;
				
			// very heavy tanks
			case "MEGATANK":
				return WEAPON.lowDamage;
				
			// ranged land units
			case "ARTILLERY":
                return 70;
			case "ANITANKCANNON":
				return WEAPON.HighDamage;
			case "MISSILE":
                return WEAPON.HighDamage;
			case "ROCKETTRHOWER":
                return WEAPON.HighDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_BAZOOKA = new Constructor();
