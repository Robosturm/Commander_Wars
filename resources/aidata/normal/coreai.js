var COREAI =
{
    infantryGroup : ["INFANTRY_GROUP", ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE", "ZCOUNIT_PARTISAN", "ZCOUNIT_COMMANDO", "ZCOUNIT_RANGER", "ZCOUNIT_AT_CYCLE"],
                                       [70,         10,     20,       30,          10,                 30,                 10,               10],
                     40, 0, "", 0.3],
    lightTankGroup : ["LIGHT_TANK_GROUP",   ["RECON", "FLARE", "LIGHT_TANK", "ARTILLERY", "ZCOUNIT_CHAPERON", "ZCOUNIT_HOT_TANK", "ZCOUNIT_SMUGGLER", "ZCOUNIT_AUTO_TANK"],
                                            [5,       5,       80,           15,          15,                 15,                 15,                 30],
                     40, 1, "", 0.3],
    mediumTankGroup : ["MEDIUM_TANK_GROUP", ["ANTITANKCANNON", "HEAVY_TANK", "ROCKETTHROWER", "ZCOUNIT_CRYSTAL_TANK", "ZCOUNIT_NEOSPIDER_TANK", "ZCOUNIT_ROYAL_GUARD", "ZCOUNIT_TANK_HUNTER"],
                                            [30,               70,           10,              40,                     40,                       40,                    40],
                     20, 2, "", 0.9],
    heavyTankGroup : ["HEAVY_TANK_GROUP", ["MEGATANK", "NEOTANK", "ZCOUNIT_SIEGE_CANNON"],
                                            [60,       30,        60,],
                     10, 3, "", 0.9],
    lightAmphibiousGroup : ["LIGHT_AMPHIBIOUS_GROUP", ["ARTILLERYCRAFT", "HOVERCRAFT", "HOVERFLAK"],
                                                      [15,               80,           10,],
                     40, 1, "", 0.3],
    mediumAmphibiousGroup : ["MEDIUM_AMPHIBIOUS_GROUP", ["HEAVY_HOVERCRAFT"],
                                                        [100,],
                     20, 2, "", 0.3],
    lightAirGroup : ["LIGHT_AIR_GROUP", ["K_HELI", "DUSTER"],
                                        [90,       10,],
                     30, 1, "", 0.3],
    heavyAirGroup : ["HEAVY_AIR_GROUP", ["BOMBER", "STEALTHBOMBER", "ZCOUNIT_KIROV"],
                                        [50,       25,              50],
                     10, 3, "", 0.9],

    navalInfantryGroup : ["NAVAL_INFANTRY_GROUP", ["GUNBOAT"],
                                                  [100],
                     10, 0, "", 0.05],
    lightNavalGroup : ["LIGHT_NAVAL_GROUP", ["CANNONBOAT", "TORPEDOBOAT"],
                                            [50,           50],
                     30, 1, "", 0.2],
    mediumNavalGroup : ["MEDIUM_NAVAL_GROUP", ["FRIGATE", "DESTROYER", "CRUISER", "SUBMARINE"],
                                              [30,        10,          30,        30],
                     50, 2, "", 0.9],
    heavyNavalGroup : ["HEAVY_NAVAL_GROUP", ["BATTLECRUISER", "BATTLESHIP", "AIRCRAFTCARRIER"],
                                            [40,              40,           5],
                     50, 3, "", 0.9],

    minInfantryTransporterMapSize : 30 * 30,
    minApcResupplyDay : 15,
    minInfTransporterDay : 3,
    minTankTransporterDay : 4,
    tankTransporterDayDifference : 3,
    transporterRatio : 0.04,

    initializeSimpleProductionSystem : function(system, ai, map)
    {
        var directIndirectRatio = ai.getAiCoBuildRatioModifier();
        var player = ai.getPlayer();
        var co1 = player.getCO(0);
        var co2 = player.getCO(1);

        system.addInitialProduction(["INFANTRY"], 6);
        system.addItemToBuildDistribution(COREAI.infantryGroup[0],  // group name
                                          COREAI.infantryGroup[1],  // units build by the group
                                          COREAI.infantryGroup[2],  // chance of the unit in this group to be build
                                          COREAI.infantryGroup[3],  // army distribution for this group
                                          COREAI.infantryGroup[4],  // build mode used to detect if the group is enabled or not to the army distribution
                                          COREAI.infantryGroup[5],  // custom condition to enable/disable group removing it to the army distribution
                                          COREAI.infantryGroup[6]);
        var variables = system.getVariables();
        var variableNavalBattle = variables.createVariable("NAVALBATTLE");
        var naval = variableNavalBattle.readDataInt32();
        var groundModifer = 1;
        if (naval > 0)
        {
            groundModifer = 1 / naval;
        }
        var variableAirBattle = variables.createVariable("AIRBATTLE");
        var air = variableAirBattle.readDataInt32();
        if (air > 0)
        {
            groundModifer *= 1 / (air > 0);
        }

        COREAI.addItemToBuildDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.lightTankGroup, groundModifer);
        COREAI.addItemToBuildDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.mediumTankGroup, groundModifer);
        COREAI.addItemToBuildDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.heavyTankGroup, groundModifer);
        COREAI.initAirForceDistribution(system, ai, co1, co2, directIndirectRatio);
        COREAI.initAmphibiousDistribution(system, ai, co1, co2, directIndirectRatio);
        if (naval > 0)
        {
            COREAI.initNavalForceDistribution(system, ai, co1, co2, directIndirectRatio);
        }
        return true;
    },

    initAirForceDistribution : function(system, ai, co1, co2, directIndirectRatio)
    {
        COREAI.addModifiedDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.lightAirGroup, "K_HELI");
        COREAI.addModifiedDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.heavyAirGroup, "BOMBER");
    },

    initNavalForceDistribution : function(system, ai, co1, co2, directIndirectRatio)
    {
        COREAI.addModifiedDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.navalInfantryGroup, "GUNBOAT");
        COREAI.addModifiedDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.lightNavalGroup, "CANNONBOAT");
        COREAI.addModifiedDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.mediumNavalGroup, "FRIGATE");
        COREAI.addModifiedDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.heavyNavalGroup, "BATTLECRUISER");
    },

    initAmphibiousDistribution : function(system, ai, co1, co2, directIndirectRatio)
    {
        COREAI.addModifiedDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.lightAmphibiousGroup, "HOVERCRAFT");
        COREAI.addModifiedDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.mediumAmphibiousGroup, "HOVERCRAFT");
    },

    addModifiedDistribution : function(system, ai, co1, co2, directIndirectRatio, group, unitId)
    {
        var dummyUnit = system.getDummyUnit(unitId);
        var mod = 0;
        if (co1 !== null)
        {
            mod += ai.getAiCoUnitMultiplier(co1, dummyUnit);
        }
        if (co2 !== null)
        {
            mod += ai.getAiCoUnitMultiplier(co2, dummyUnit);
        }
        var multiplier = 1;
        if (mod > 2)
        {
            multiplier = 3;
        }
        COREAI.addItemToBuildDistribution(system, ai, co1, co2, directIndirectRatio, group, multiplier);
    },

    addItemToBuildDistribution : function(system, ai, co1, co2, directIndirectRatio, group, distributionModifier)
    {
        var chances = [...group[2]];

        var baseValue = 0;
        if (co1 !== null)
        {
            baseValue += 5;
        }
        if (co2 !== null)
        {
            baseValue += 5;
        }

        for (var i = 0; i < chances.length; ++i)
        {
            var dummyUnit = system.getDummyUnit(group[1][i]);
            var unitRatioModifier = 1.0;
            if (directIndirectRatio > 0)
            {
                if (dummyUnit.getBaseMinRange() > 1)
                {
                    unitRatioModifier = 1 / directIndirectRatio;
                }
                else
                {
                    unitRatioModifier = directIndirectRatio;
                }
            }
            var coModifier = 1.0;
            if (baseValue > 0)
            {
                var mod = 0;
                if (co1 !== null)
                {
                    mod += ai.getAiCoUnitMultiplier(co1, dummyUnit);
                }
                if (co2 !== null)
                {
                    mod += ai.getAiCoUnitMultiplier(co2, dummyUnit);
                }
                coModifier += mod / baseValue;
            }
            chances[i] *= unitRatioModifier * coModifier;
        }
        system.addItemToBuildDistribution(group[0],                         // group name
                                          group[1],                         // units build by the group
                                          chances,                          // chance of the unit in this group to be build
                                          group[3] * distributionModifier,  // army distribution for this group
                                          group[4],                         // build mode used to detect if the group is enabled or not to the army distribution
                                          group[5],                         // custom condition to enable/disable group removing it to the army distribution
                                          group[6]);

    },
    onNewBuildQueue : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        system.resetForcedProduction();
        COREAI.forceAntiAirProduction(system, ai, units, enemyUnits);
        COREAI.forceTransporterProduction(system, ai, buildings, units, enemyUnits, enemyBuildings);
        COREAI.forceApcProduction(system, ai, units);
    },

    forceTransporterProduction  : function(system, ai, buildings, units, enemyUnits, enemyBuildings)
    {
        var map = ai.getMap();
        var turn = map.getCurrentDay();
        if (turn >= COREAI.minInfTransporterDay)
        {
            var transporterBuildings = ai.getBuildingCountsOnEnemyIslands(units, enemyBuildings);
            var infantryTransporter = ["T_HELI", "BLACK_BOAT"];
            var infantryTransporterCount = ai.getUnitCount(units, infantryTransporter);
            if (infantryTransporterCount === 0 &&
                    (COREAI.minInfantryTransporterMapSize <= map.getMapWidth() * map.getMapHeight() ||
                     transporterBuildings > 0))
            {
                if (transporterBuildings > 0)
                {
                    system.addForcedProduction(infantryTransporter);
                }
                else
                {
                    infantryTransporter.push("APC");
                    system.addForcedProduction(infantryTransporter);
                }
            }
        }
        if (turn >= COREAI.minTankTransporterDay)
        {
            var tankTransporter = ["LANDER", "TRANSPORTPLANE"];
            var tankTransporterCount = ai.getUnitCount(units, tankTransporter);
            var idleUnitCount = ai.getIdleUnitCount(units, [], enemyUnits, enemyBuildings);
            var unitCount = units.size();
            var variables = system.getVariables();
            var variableNavalBattle = variables.createVariable("NAVALBATTLE");
            var naval = variableNavalBattle.readDataInt32();
            var variableAirBattle = variables.createVariable("AIRBATTLE");
            var air = variableAirBattle.readDataInt32();
            var variableLastTransporterDay = variables.createVariable("LASTTRANSPORTERDAY");
            var lastTransporterDay = variableLastTransporterDay.readDataInt32();
            if (naval === 0 && air === 0 && idleUnitCount > 0)
            {
                var harbourCount = buildings.getBuildingCount("HARBOUR") + enemyBuildings.getBuildingCount("HARBOUR");
                var airportCount = buildings.getBuildingCount("AIRPORT") + enemyBuildings.getBuildingCount("AIRPORT");
                if (harbourCount > 0)
                {
                    naval += 1;
                    if (airportCount === 0)
                    {
                        naval += 2;
                    }
                    variableNavalBattle.writeDataInt32(naval);
                }
                if (airportCount > 0)
                {
                    air += 1;
                    if (harbourCount === 0)
                    {
                        air += 2;
                    }
                    variableAirBattle.writeDataInt32(air);
                }
                system.resetBuildDistribution();
                COREAI.initializeSimpleProductionSystem(system, ai, map);
            }
            tankTransporter = [];
            if (naval > 0)
            {
                tankTransporter.push("LANDER");
            }
            if (air > 0)
            {
                tankTransporter.push("TRANSPORTPLANE");
            }
            if (tankTransporter.length > 0 &&
               (lastTransporterDay === 0 || turn >= COREAI.tankTransporterDayDifference + lastTransporterDay))
            {
                if (COREAI.transporterRatio * unitCount > tankTransporterCount)
                {
                    system.addForcedProduction(tankTransporter);
                    variableLastTransporterDay.writeDataInt32(turn);
                }
            }
        }
    },

    forceApcProduction  : function(system, ai, units)
    {
        var map = ai.getMap();
        var turn = map.getCurrentDay();
        var lowFuelUnitCount = ai.getUnitCount(units, [], 5, 30);
        var apcCount = ai.getUnitCount(units, ["APC"]);
        if (apcCount === 0 &&
            turn >= COREAI.minApcResupplyDay &&
            (lowFuelUnitCount > 2 ||
             COREAI.minInfantryTransporterMapSize <= map.getMapWidth() * map.getMapHeight()))
        {
            system.addForcedProduction(["APC"]);
        }
    },

    forceAntiAirProduction : function(system, ai, units, enemyUnits)
    {
        var antiAirUnits = ["FLAK", "FIGHTER", "MISSILE"];
        var airUnits = ["K_HELI", "DUSTER", "BOMBER", "ZCOUNIT_KIROV", "FIGHTER", "WATERPLANE"];
        var stealthAirUnits = ["STEALTHBOMBER"]
        var antiAirAirUnits = ["FIGHTER", "STEALTHBOMBER", "DUSTER"];
        var variables = system.getVariables();
        var variableNavalBattle = variables.createVariable("NAVALBATTLE");
        var naval = variableNavalBattle.readDataInt32();
        if (naval > 1)
        {
            antiAirUnits.push("CRUISER");
        }
        var anitAirUnitCount = ai.getUnitCount(units, antiAirUnits, 5);
        var anitAirAirUnitCount = ai.getUnitCount(units, antiAirAirUnits, 5);
        var stealthBomberUnitCount = ai.getEnemyUnitCountNearOwnUnits(units, enemyUnits, stealthAirUnits, 24, 5);
        var enemyAirUnits = ai.getEnemyUnitCountNearOwnUnits(units, enemyUnits, airUnits, 24, 5);
        if ((enemyAirUnits > 0 && anitAirUnitCount === 0) ||
            (enemyAirUnits > 2 && anitAirUnitCount === 1) ||
            (anitAirUnitCount > 0 && enemyAirUnits / anitAirUnitCount >= 3))
        {
            system.addForcedProduction(antiAirUnits);
        }
        if ((stealthAirUnits > 0 && anitAirAirUnitCount === 0) ||
            (stealthAirUnits > 2 && anitAirAirUnitCount === 1) ||
            (anitAirAirUnitCount > 0 && stealthAirUnits / anitAirAirUnitCount >= 3))
        {
            system.addForcedProduction(antiAirAirUnits);
        }
    },

    fundsModes : [[0,    0,  0, 0],
                  [2, 12000, 0, 1],
                  [3, 9000,  0, 1],
                  [3, 18000, 0, 2],
                  [0, 22000, 0, 3],
                  [6, 22000, 1, 3],
                  [6, 30000, 2, 3],],

    buildUnitSimpleProductionSystem : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        var turn = map.getCurrentDay();
        var player = ai.getPlayer();
        var funds = player.getFunds();
        var minMode = 0;
        var maxMode = 0;
        for (var i = COREAI.fundsModes.length - 1; i >= 0; --i)
        {
            if (turn >= COREAI.fundsModes[i][0] &&
                funds >= COREAI.fundsModes[i][1])
            {
                minMode = COREAI.fundsModes[i][2];
                maxMode = COREAI.fundsModes[i][3];
                break;
            }
        }
        return system.buildNextUnit(buildings, units, minMode, maxMode);
    },
};
