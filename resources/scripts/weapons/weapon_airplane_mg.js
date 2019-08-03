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
    this.damageTable = [["INFANTRY", 120],
                        ["MECH", 45],
                        ["MOTORBIKE", 45],
                        ["SNIPER", 45],
                        // supporter
                        ["APC", 15],
                        ["FLARE", 15],
                        ["RECON", 18],
                        // tanks
                        ["FLAK", 5],
                        ["HOVERFLAK", 5],
                        ["LIGHT_TANK", 8],
                        ["HOVERCRAFT", 8],
                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 3],
                        ["HEAVY_TANK", 3],
                        ["NEOTANK", 3],
                        // very heavy tanks
                        ["MEGATANK", 1],
                        ["HOELLIUM", 20],
                        // ranged land units
                        ["ARTILLERY", 15],
                        ["ARTILLERYCRAFT", 15],
                        ["ANTITANKCANNON", 25],
                        ["MISSILE", 20],
                        ["ROCKETTHROWER", 20],
                        ["PIPERUNNER", 6],
                        // air
                        ["DUSTER", 55],
                        ["FIGHTER", 40],
                        ["BOMBER", 45],
                        ["STEALTHBOMBER", 45],
                        ["TRANSPORTPLANE", 65],
                        ["BLACK_BOMB", 65],
                        ["WATERPLANE", 45],
                        ["K_HELI", 85],
                        ["T_HELI", 95]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_AIRPLANE_MG.damageTable);
    };
};

Constructor.prototype = WEAPON;
var WEAPON_AIRPLANE_MG = new Constructor();
