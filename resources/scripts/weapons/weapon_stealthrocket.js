var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 70;
    };
    this.damageTable = [["INFANTRY", 105],
                        ["MECH", 85],
                        ["MOTORBIKE", 85],
                        ["SNIPER", 85],

                        // supporter
                        ["APC", 75],
                        ["FLARE", 75],
                        ["RECON", 75],

                        // tanks
                        ["FLAK", 35],
                        ["HOVERFLAK", 35],
                        ["LIGHT_TANK", 65],
                        ["HOVERCRAFT", 65],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 55],
                        ["HEAVY_TANK", 45],
                        ["NEOTANK", 55],

                        // very heavy tanks
                        ["MEGATANK", 35],

                        ["HOELLIUM", 30],

                        // ranged land units
                        ["ARTILLERY", 65],
                        ["ARTILLERYCRAFT", 65],
                        ["ANTITANKCANNON", 55],
                        ["MISSILE", 55],
                        ["ROCKETTHROWER", 75],
                        ["PIPERUNNER", 75],

                        // air units
                        ["DUSTER", 65],
                        ["FIGHTER", 45],
                        ["BOMBER", 75],
                        ["STEALTHBOMBER", 75],
                        ["TRANSPORTPLANE", 120],
                        ["BLACK_BOMB", 120],
                        ["WATERPLANE", 55],
                        ["K_HELI", 85],
                        ["T_HELI", 95],


                        // ships
                        ["BATTLESHIP", 55],
                        ["CANNONBOAT", 75],
                        ["CRUISER", 35],
                        ["DESTROYER", 35],
                        ["SUBMARINE", 55],
                        ["LANDER", 55],
                        ["BLACK_BOAT", 75],
                        ["AIRCRAFTCARRIER", 55]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_STEALTHROCKET.damageTable, "WEAPON_STEALTHROCKET");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_STEALTHROCKET = new Constructor();
