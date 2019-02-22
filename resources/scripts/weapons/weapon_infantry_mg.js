var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("MG");
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
        case "INFANTRY":
            return WEAPON.mediumDamage;
        case "MECH":
            return WEAPON.lowDamage;
        case "MOTORBIKE":
            return WEAPON.HighDamage;
        case "SNIPER":
            return WEAPON.HighDamage;

        case "APC":
            return 10;
        case "FLARE":
            return 10;
        case "RECON":
            return 10;

        case "FLAK":
            return 5;
        case "HOVERFLAK":
            return 5;
        case "LIGHT_TANK":
            return 5;
        case "HOVERFLAK":
            return 5;

        case "HEAVY_HOVERCRAFT":
            return 3;
        case "HEAVY_TANK":
            return 3;
        case "NEOTANK":
            return 3;

        case "MEGATANK":
            return 1;

        case "T_HELI":
            return 30;
        case "K_HELI":
            return 10;

        case "ARTILLERY":
            return 10;
        case "ANITANKCANNON":
            return 30;
        case "MISSILE":
            return 20;
        case "ROCKETTRHOWER":
            return 20;
        default:
            return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_INFANTRY_MG = new Constructor();
