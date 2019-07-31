var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Vulcan Cannon");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 1;
    };
    this.damageTable = [["INFANTRY", 170],
                        ["MECH", 150],
                        ["MOTORBIKE", 150],
                        ["SNIPER", 150],

                        // supporter
                        ["APC", 50],
                        ["FLARE", 45],
                        ["RECON", 60],

                        // tanks
                        ["FLAK", 45],
                        ["HOVERFLAK", 45],
                        ["LIGHT_TANK", 15],
                        ["HOVERCRAFT", 15],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 10],
                        ["HEAVY_TANK", 10],
                        ["NEOTANK", 10],

                        // very heavy tanks
                        ["MEGATANK", 5],

                        ["HOELLIUM", 30],

                        // heli copter
                        ["T_HELI", 120],
                        ["K_HELI", 105],

                        //air
                        ["DUSTER", 75],
                        ["FIGHTER", 75],
                        ["BOMBER", 75],
                        ["STEALTHBOMBER", 75],
                        ["BLACK_BOMB", 75],
                        ["TRANSPORTPLANE", 75],
                        ["WATERPLANE", 75],

                        // ranged land units
                        ["ARTILLERY", 50],
                        ["ANTITANKCANNON", 25],
                        ["MISSILE", 55],
                        ["ROCKETTHROWER", 55],
                        ["PIPERUNNER", 25]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_HOVERVULCAN_CANNON.damageTable);
    };
};

Constructor.prototype = WEAPON;
var WEAPON_HOVERVULCAN_CANNON = new Constructor();
