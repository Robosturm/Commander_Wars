var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 55;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
            // supporter
            case "APC":
                return 105;
            case "FLARE":
                return 105;
            case "RECON":
                return 105;

            // tanks
            case "FLAK":
                return 105;
            case "HOVERFLAK":
                return 105;
            case "LIGHT_TANK":
                return 85;
            case "HOVERCRAFT":
                return 85;

            // heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 55;
            case "HEAVY_TANK":
                return 55;
            case "NEOTANK":
                return 45;

            // very heavy tanks
            case "MEGATANK":
                return 25;

            case "HOELLIUM":
                return 30;

            // ranged land units
            case "ARTILLERY":
                return 105;
            case "ANTITANKCANNON":
                return 35;
            case "MISSILE":
                return 105;
            case "ROCKETTHROWER":
                return 105;

            // ships
            case "BATTLESHIP":
                return 10;
            case "CANNONBOAT":
                return 35;
            case "CRUISER":
                return 30;
            case "DESTROYER":
                return 30;
            case "SUBMARINE":
                return 10;
            case "LANDER":
                return 35;
            case "BLACK_BOAT":
                return 35;
            case "AIRCRAFTCARRIER":
                return 10;
            default:
                return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_HEAVY_TANK_GUN = new Constructor();
