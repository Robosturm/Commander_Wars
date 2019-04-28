var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 45;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// infantry
			case "INFANTRY":
                return 90;
			case "MECH":
                return 85;
			case "MOTORBIKE":
                return 85;
			case "SNIPER":
                return 85;

			// supporter
			case "APC":
                return 75;
			case "FLARE":
                return 80;
			case "RECON":
                return 80;

			// tanks
			case "FLAK":
                return 45;
			case "HOVERFLAK":
                return 45;
			case "LIGHT_TANK":
                return 75;
			case "HOVERCRAFT":
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

            case "HOELLIUM":
                return 20;
				
			// ranged land units
			case "ARTILLERY":
                return 70;
			case "ANTITANKCANNON":
                return 50;
			case "MISSILE":
                return 70;
            case "ROCKETTHROWER":
                return 80;
			
			case "DUSTER":
                return 65;
			case "FIGHTER":
                return 45;
			case "BOMBER":
                return 55;
			case "STEALTHBOMBER":
                return 55;
			case "TRANSPORTPLANE":
                return 75;
            case "BLACK_BOAT":
                return 75;
			case "WATERPLANE":
                return 55;
			case "K_HELI":
                return 85;
			case "T_HELI":
                return 95;
				
			// ships
			case "BATTLESHIP":
                return 45;
			case "CANNONBOAT":
				return 105;				
            case "BLACK_BOAT":
                return 105;
			case "CRUISER":
                return 40;
			case "DESTROYER":
                return 40;
			case "SUBMARINE":
                return 55;
			case "LANDER":
                return 85;
			case "AIRCRAFTCARRIER":
                return 65;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_WATERPLANE_MISSILE = new Constructor();
