var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// supporter
			case "APC":
				return 90;
			case "FLARE":
				return 90;
			case "RECON":
				return 95;

			// tanks
			case "FLAK":
                return 90;
			case "HOVERFLAK":
                return 90;
			case "LIGHT_TANK":
                return 70;
            case "HOVERTANK":
                return 70;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 45;
			case "HEAVY_TANK":
                return WEAPON.HighDamage;
			case "NEOTANK":
                return WEAPON.mediumDamage;
				
			// very heavy tanks
			case "MEGATANK":
				return WEAPON.softCounterDamage;
				
			// ranged land units
			case "ARTILLERY":
                return WEAPON.HighDamage;
			case "ANITANKCANNON":
				return WEAPON.softCounterDamage;
			case "MISSILE":
                return 95;
			case "ROCKETTRHOWER":
                return 95;
			
			// ships
			case "BATTLESHIP":
				return WEAPON.lowDamage;
			case "CANNONBOAT":
				return WEAPON.mediumDamage;
			case "CRUISER":
				return WEAPON.lowDamage;
			case "DESTROYER":
				return WEAPON.lowDamage;
			case "SUBMARINE":
				return WEAPON.lowDamage;
			case "LANDER":
				return WEAPON.lowDamage;
			case "AIRCRAFTCARRIER":
				return WEAPON.lowDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_NEOTANK_GUN = new Constructor();
