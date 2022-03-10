var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 45;
    };
    this.damageTable = [["INFANTRY", 110],
                        ["MECH", 85],
                        ["MOTORBIKE", 85],
                        ["SNIPER", 85],

                        // supporter
                        ["APC", 75],
                        ["FLARE", 80],
                        ["RECON", 80],

                        // tanks
                        ["FLAK", 45],
                        ["HOVERFLAK", 45],
                        ["LIGHT_TANK", 75],
                        ["HOVERCRAFT", 75],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 65],
                        ["HEAVY_TANK", 55],
                        ["NEOTANK", 65],

                        // very heavy tanks
                        ["MEGATANK", 45],

                        ["HOELLIUM", 20],

                        // ranged land units
                        ["ARTILLERY", 70],
                        ["ARTILLERYCRAFT", 70],
                        ["ANTITANKCANNON", 50],
                        ["MISSILE", 70],
                        ["ROCKETTHROWER", 80],
                        ["PIPERUNNER", 80],

                        ["DUSTER", 65],
                        ["FIGHTER", 45],
                        ["BOMBER", 55],
                        ["STEALTHBOMBER", 55],
                        ["TRANSPORTPLANE", 75],
                        ["BLACK_BOMB", 75],
                        ["WATERPLANE", 55],
                        ["K_HELI", 85],
                        ["T_HELI", 95],

                        // ships
                        ["BATTLESHIP", 45],
                        ["CANNONBOAT", 105],
                        ["CRUISER", 40],
                        ["DESTROYER", 40],
                        ["SUBMARINE", 55],
                        ["LANDER", 105],
                        ["BLACK_BOAT", 105],
                        ["AIRCRAFTCARRIER", 65],
                        ["WATERMINE", 400]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_WATERPLANE_MISSILE.damageTable, "WEAPON_WATERPLANE_MISSILE");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_WATERPLANE_MISSILE = new Constructor();
