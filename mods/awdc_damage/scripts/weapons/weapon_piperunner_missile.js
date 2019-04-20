var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
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
                return 85;
			case "MECH":
                return 85;
			case "MOTORBIKE":
                return 85;
			case "SNIPER":
                return 85;

			// supporter
			case "APC":
				return 80;
			case "FLARE":
				return 80;
			case "RECON":
				return 80;

			// tanks
			case "FLAK":
                return 65;
			case "HOVERFLAK":
                return 65;
			case "LIGHT_TANK":
                return 65;
            case "HOVERCRAFT":
                return 65;
				
			// heavy tanks
			case "HEAVY_HOVERCRAFT":
                return 55;
			case "HEAVY_TANK":
                return 55;
			case "NEOTANK":
                return 55;
				
			// very heavy tanks
			case "MEGATANK":
                return 35;
				
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
                return 85;
			case "ANTITANKCANNON":
                return 55;
			case "MISSILE":
                return 85;
            case "ROCKETTHROWER":
                return 85;
				
			// ships
			case "BATTLESHIP":
                return 55;
			case "CANNONBOAT":
				return 75;				
			case "CRUISER":
                return 55;
			case "DESTROYER":
                return 55;
			case "SUBMARINE":
                return 55;
			case "LANDER":
                return 55;
			case "AIRCRAFTCARRIER":
                return 55;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_PIPERUNNER_MISSILE = new Constructor();
