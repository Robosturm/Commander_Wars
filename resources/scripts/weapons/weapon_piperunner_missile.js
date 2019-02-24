var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// infantry
			case "INFANTRY":
				return WEAPON.HighDamage;
			case "MECH":
				return WEAPON.HighDamage;
			case "MOTORBIKE":
				return WEAPON.HighDamage;
			case "SNIPER":
				return WEAPON.HighDamage;

			// supporter
			case "APC":
				return 80;
			case "FLARE":
				return 80;
			case "RECON":
				return 80;

			// tanks
			case "FLAK":
				return WEAPON.softDamage;
			case "HOVERFLAK":
				return WEAPON.softDamage;
			case "LIGHT_TANK":
				return WEAPON.softDamage;
            case "HOVERTANK":
				return WEAPON.softDamage;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
				return WEAPON.mediumDamage;
			case "HEAVY_TANK":
				return WEAPON.mediumDamage;
			case "NEOTANK":
				return WEAPON.mediumDamage;
				
			// very heavy tanks
			case "MEGATANK":
				return WEAPON.lowDamage;
				
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
			case "TRANSPORTPLANE":
				return 75;
			case "WATERPLANE":
				return 75;
				
			// ranged land units
			case "ARTILLERY":
				return WEAPON.HighDamage;
			case "ANITANKCANNON":
                return WEAPON.mediumDamage;
			case "MISSILE":
                return WEAPON.HighDamage;
			case "ROCKETTRHOWER":
				return WEAPON.HighDamage;
				
			// ships
			case "BATTLESHIP":
				return WEAPON.mediumDamage;
			case "CANNONBOAT":
				return 75;				
			case "CRUISER":
				return WEAPON.mediumDamage;
			case "DESTROYER":
				return WEAPON.mediumDamage;
			case "SUBMARINE":
				return WEAPON.mediumDamage;				
			case "LANDER":
				return WEAPON.mediumDamage;
			case "AIRCRAFTCARRIER":
                return WEAPON.mediumDamage;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_PIPERUNNER_MISSILE = new Constructor();
