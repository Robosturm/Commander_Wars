var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
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
                return 80;
            case "RECON":
                return 85;

                // tanks
            case "FLAK":
                return 75;
            case "HOVERFLAK":
                return 75;
            case "LIGHT_TANK":
                return 55;
            case "HOVERCRAFT":
                return 55;

                // heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 35;
            case "HEAVY_TANK":
                return 35;
            case "NEOTANK":
                return 35;

                // very heavy tanks
            case "MEGATANK":
                return 20;

            case "HOELLIUM":
                return 20;

                // ranged land units
            case "ARTILLERY":
                return 70;
            case "ANTITANKCANNON":
                return 30;
            case "MISSILE":
                return 85;
            case "ROCKETTHROWER":
                return 85;

                // ships
            case "BATTLESHIP":
                return 8;
            case "CANNONBOAT":
                return 55;
            case "BLACK_BOAT":
                return 55;
            case "CRUISER":
                return 9;
            case "DESTROYER":
                return 9;
            case "SUBMARINE":
                return 9;
            case "LANDER":
                return 18;
            case "AIRCRAFTCARRIER":
                return 8;
            default:
                return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_LIGHT_TANK_GUN = new Constructor();
