var Constructor = function()
{
    this.damageTable = [["DUSTER", 105],
                        ["FIGHTER", 105],
                        ["BOMBER", 105],
                        ["BLACK_BOMB", 105],
                        ["STEALTHBOMBER", 105],
                        ["TRANSPORTPLANE", 105],
                        ["WATERPLANE", 105],
                        ["K_HELI", 120],
                        ["T_HELI", 120],

                        // infantry
                        ["INFANTRY", 115],
                        ["MECH", 70],
                        ["MOTORBIKE", 70],
                        ["SNIPER", 70],

                        // supporter
                        ["APC", 45],
                        ["FLARE", 45],
                        ["RECON", 45],

                        // tanks
                        ["FLAK", 8],
                        ["HOVERFLAK", 8],
                        ["LIGHT_TANK", 8],
                        ["HOVERCRAFT", 8],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 5],
                        ["HEAVY_TANK", 5],
                        ["NEOTANK", 5],

                        // very heavy tanks
                        ["MEGATANK", 1],

                        ["HOELLIUM", 20],

                        // ranged land units
                        ["ARTILLERY", 45],
                        ["ARTILLERYCRAFT", 45],
                        ["ANTITANKCANNON", 1],
                        ["MISSILE", 55],
                        ["ROCKETTHROWER", 55],
                        ["PIPERUNNER", 6]];

    this.getName = function()
    {
        return qsTr("Anti Air Gun");
    };
    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_A_AIR_GUN.damageTable, "WEAPON_A_AIR_GUN");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_A_AIR_GUN = new Constructor();
