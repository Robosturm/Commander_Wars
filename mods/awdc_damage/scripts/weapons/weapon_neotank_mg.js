var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("MG");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 1;
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
            // infantry
            case "INFANTRY":
                return 90;
            case "MECH":
                return 80;
            case "MOTORBIKE":
                return 80;
            case "SNIPER":
                return 80;

            // supporter
            case "APC":
                return 45;
            case "FLARE":
                return 35;
            case "RECON":
                return 40;

            // tanks
            case "FLAK":
                return 8;
            case "HOVERFLAK":
                return 8;
            case "LIGHT_TANK":
                return 8;
            case "HOVERTANK":
                return 8;

            // heavy tanks
            case "HEAVY_HOVERCRAFT":
                return 5;
            case "HEAVY_TANK":
                return 5;
            case "NEOTANK":
                return 5;

            // very heavy tanks
            case "MEGATANK":
                return 1;

            // heli copter
            case "T_HELI":
                return 40;
            case "K_HELI":
                return 24;

            // ranged land units
            case "ARTILLERY":
                return 45;
            case "ANTITANKCANNON":
                return 1;
            case "MISSILE":
                return 60;
            case "ROCKETTHROWER":
                return 60;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_NEOTANK_MG = new Constructor();
