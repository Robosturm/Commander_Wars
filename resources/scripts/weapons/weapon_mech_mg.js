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
            return WEAPON.HighDamage;
        case "MECH":
            return WEAPON.mediumDamage;
        case "MOTORBIKE":
            return WEAPON.lowDamage;
        case "SNIPER":
            return WEAPON.HighDamage;

        case "APC":
            return 20;
        case "FLARE":
            return 20;
        case "RECON":
            return 20;

        case "FLAK":
            return 5;
        case "HOVERFLAK":
            return 5;
        case "LIGHT_TANK":
            return 5;
        case "HOVERTANK":
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
            return WEAPON.lowDamage;
        case "ANITANKCANNON":
            return 45;
        case "MISSILE":
            return WEAPON.softCounterDamage;
        case "ROCKETTRHOWER":
            return WEAPON.softCounterDamage;
        default:
            return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MECH_MG = new Constructor();
