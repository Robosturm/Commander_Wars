 Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Cannon");
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
                return 70;
			case "FLARE":
                return 80;
			case "RECON":
                return 80;

			// tanks
			case "FLAK":
                return 75;
			case "HOVERFLAK":
                return 75;
			case "LIGHT_TANK":
                return 70;
            case "HOVERCRAFT":
                return 70;
				
			// heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 45;
			case "HEAVY_TANK":
                return 45;
			case "NEOTANK":
                return 40;
				
			// very heavy tanks
			case "MEGATANK":
                return 15;
				
			// ranged land units
			case "ARTILLERY":
                return 75;
            case "ANTITANKCANNON":
                return 55;
			case "MISSILE":
                return 80;
            case "ROCKETTHROWER":
                return 80;

            case "HOELLIUM":
                return 5;
			
			// ships
			case "BATTLESHIP":
                return 55;
			case "CANNONBOAT":
                return 60;
			case "CRUISER":
                return 60;
			case "DESTROYER":
                return 55;
			case "SUBMARINE":
                return 85;
			case "LANDER":
                return 60;
            case "BLACK_BOAT":
                return 60;
			case "AIRCRAFTCARRIER":
                return 60;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
 WEAPON_CANNON = new Constructor();
