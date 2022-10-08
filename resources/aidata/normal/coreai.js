var COREAI =
{
    infantryGroup : ["INFANTRY_GROUP", ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE", "ZCOUNIT_PARTISAN", "ZCOUNIT_COMMANDO", "ZCOUNIT_RANGER", "ZCOUNIT_AT_CYCLE"],
                                       [70,         10,     20,       30,          10,                 30,                 10,               10],
                     40, 0, ""],
    lightTankGroup : ["LIGHT_TANK_GROUP",   ["RECON", "FLARE", "LIGHT_TANK", "ARTILLERY", "ZCOUNIT_CHAPERON", "ZCOUNIT_HOT_TANK", "ZCOUNIT_SMUGGLER", "ZCOUNIT_AUTO_TANK"],
                                            [5,       5,       80,           15,          15,                 15,                 15,                 30],
                     40, 1, ""],
    mediumTankGroup : ["MEDIUM_TANK_GROUP", ["ANTITANKCANNON", "HEAVY_TANK", "ROCKETTHROWER", "ZCOUNIT_CRYSTAL_TANK", "ZCOUNIT_NEOSPIDER_TANK", "ZCOUNIT_ROYAL_GUARD", "ZCOUNIT_TANK_HUNTER"],
                                            [30,               70,           10,              40,                     40,                       40,                    40],
                     20, 2, ""],
    heavyTankGroup : ["HEAVY_TANK_GROUP", ["MEGATANK", "NEOTANK", "ZCOUNIT_SIEGE_CANNON"],
                                            [60,       30,        60,],
                     10, 3, ""],
    lightAmphibiousGroup : ["LIGHT_AMPHIBIOUS_GROUP", ["ARTILLERYCRAFT", "HOVERCRAFT", "HOVERFLAK"],
                                                      [15,               80,           10,],
                     40, 1, ""],
    mediumAmphibiousGroup : ["MEDIUM_AMPHIBIOUS_GROUP", ["HEAVY_HOVERCRAFT",],
                                                        [100,],
                     20, 2, ""],
    lightAirForce : ["LIGHT_AIR_GROUP", ["K_HELI", "DUSTER",],
                                        [90,       10,],
                                        30, 1, ""],
    heavyAirForce : ["HEAVY_AIR_GROUP", ["BOMBER", "FIGHTER", "ZCOUNIT_KIROV"],
                                        [60,       10,        60],
                                        10, 3, ""],
    initializeSimpleProductionSystem : function(system, ai, map)
    {
        var directIndirectRatio = ai.getAiCoBuildRatioModifier();
        var player = ai.getPlayer();
        var co1 = player.getCO(0);
        var co2 = player.getCO(1);

        system.addInitialProduction("INFANTRY", 6);
        system.addItemToBuildDistribution(COREAI.infantryGroup[0],  // group name
                                          COREAI.infantryGroup[1],  // units build by the group
                                          COREAI.infantryGroup[2],  // chance of the unit in this group to be build
                                          COREAI.infantryGroup[3],  // army distribution for this group
                                          COREAI.infantryGroup[4],  // build mode used to detect if the group is enabled or not to the army distribution
                                          COREAI.infantryGroup[5]); // custom condition to enable/disable group removing it to the army distribution
        COREAI.addItemToBuildDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.lightTankGroup);
        COREAI.addItemToBuildDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.mediumTankGroup);
        COREAI.addItemToBuildDistribution(system, ai, co1, co2, directIndirectRatio, COREAI.heavyTankGroup);
        return true;
    },
    addItemToBuildDistribution : function(system, ai, co1, co2, directIndirectRatio, group)
    {
        var chances = [...group[2]];
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
            chances[i] *= unitRatioModifier;
        }
        system.addItemToBuildDistribution(group[0],   // group name
                                          group[1],      // units build by the group
                                          chances,    // chance of the unit in this group to be build
                                          group[3],   // army distribution for this group
                                          group[4],   // build mode used to detect if the group is enabled or not to the army distribution
                                          group[5]);  // custom condition to enable/disable group removing it to the army distribution

    },
    onNewBuildQueue : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        system.resetForcedProduction();

    },
    buildUnitSimpleProductionSystem : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        // todo add force anti air production
        // todo add force transporter production
        // todo add apc production

        return system.buildNextUnit(buildings, units, 0, 1);
    },
};
