var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 75;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
            // supporter
            case "APC":
                return 125;
            case "FLARE":
                return 125;
            case "RECON":
                return 125;

            // tanks
            case "FLAK":
                return 115;
            case "HOVERFLAK":
                return 115;
            case "LIGHT_TANK":
                return 105;
            case "HOVERCRAFT":
                return 105;

            // heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 75;
            case "HEAVY_TANK":
                return 75;
            case "NEOTANK":
                return 55;

            // very heavy tanks
            case "MEGATANK":
                return 35;

            case "HOELLIUM":
                return 20;

            // ranged land units
            case "ARTILLERY":
                return 115;
            case "ANTITANKCANNON":
                return 45;
            case "MISSILE":
                return 125;
            case "ROCKETTHROWER":
                return 125;

            // ships
            case "BATTLESHIP":
                return 15;
            case "CANNONBOAT":
                return 40;
            case "CRUISER":
                return 30;
            case "DESTROYER":
                return 30;
            case "SUBMARINE":
                return 15;
            case "LANDER":
                return 40;
            case "BLACK_BOAT":
                return 40;
            case "AIRCRAFTCARRIER":
                return 15;
            default:
                return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_NEOTANK_GUN = new Constructor();
