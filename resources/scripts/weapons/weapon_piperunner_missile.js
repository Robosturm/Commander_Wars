var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 55;
    };
    this.damageTable = [["INFANTRY", 125],
                        ["MECH", 85],
                        ["MOTORBIKE", 85],
                        ["SNIPER", 85],

                        // supporter
                        ["APC", 80],
                        ["FLARE", 80],
                        ["RECON", 80],

                        // tanks
                        ["FLAK", 65],
                        ["HOVERFLAK", 65],
                        ["LIGHT_TANK", 65],
                        ["HOVERCRAFT", 65],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 55],
                        ["HEAVY_TANK", 405],
                        ["NEOTANK", 55],

                        // very heavy tanks
                        ["MEGATANK", 35],

                        ["HOELLIUM", 15],

                        // heli copter
                        ["T_HELI", 120],
                        ["K_HELI", 105],

                        //air
                        ["DUSTER", 75],
                        ["FIGHTER", 75],
                        ["BOMBER", 75],
                        ["STEALTHBOMBER", 75],
                        ["TRANSPORTPLANE", 75],
                        ["BLACK_BOMB", 75],
                        ["WATERPLANE", 75],

                        // ranged land units
                        ["ARTILLERY", 75],
                        ["ARTILLERYCRAFT", 75],
                        ["ANTITANKCANNON", 55],
                        ["MISSILE", 85],
                        ["ROCKETTHROWER", 85],
                        ["PIPERUNNER", 85],

                        // ships
                        ["BATTLESHIP", 55],
                        ["CANNONBOAT", 75],
                        ["CRUISER", 55],
                        ["DESTROYER", 55],
                        ["SUBMARINE", 55],
                        ["LANDER", 55],
                        ["BLACK_BOAT", 75],
                        ["AIRCRAFTCARRIER", 55]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_PIPERUNNER_MISSILE.damageTable, "WEAPON_PIPERUNNER_MISSILE");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_PIPERUNNER_MISSILE = new Constructor();
