var Constructor = function()
{
    this.getCOStyles = function()
    {
        return [];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(3);
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);

            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            animation.setSound("power1.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);

            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 7)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power12_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power12_2.wav", 1, delay);
            }
            if (animations.length < 7)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/andy.mp3",  4466, 74972);
                break;
            }
        }
    };
    this.getCOUnitRange = function(co, map)
    {
        return 4;
    };
    this.getCOArmy = function()
    {
        return "BD";
    };

    this.wilderness = ["DESERT_FOREST",
                       "DESERT_FOREST1",
                       "DESERT_ROCK",
                       "DESERT_TRY_RIVER",
                       "FOREST",
                       "FOREST1",
                       "FOREST2",
                       "FOREST3",
                       "RIVER",
                       "MOUNTAIN",
                       "SNOW_FOREST",
                       "SNOW_FOREST1",
                       "SNOW_FOREST2",
                       "SNOW_MOUNTAIN",
                       "WASTE_FOREST",
                       "WASTE_MOUNTAIN"];

    this.isWildernessTile = function(x, y, map)
    {
        if (map !== null)
        {
            if (map.onMap(x, y))
            {
                var terrain = map.getTerrain(x, y);
                var id = terrain.getID();
                if (CO_CAIRN.wilderness.includes(id))
                {
                    return true;
                }
            }
        }
        return false;
    };

    this.superPowerHeal = 1;
    this.superPowerDefenseReduction = 10;
    this.superPowerOffBonus = 10;

    this.powerTerrainBonus = 2;
    this.powerFirerangeBonus = 1;
    this.powerMovementCostReduction = -1;
    this.powerVisionrangeModifier = 1;
    this.powerDefBonus = 10;
    this.powerOffBonus = 10;

    this.d2dBuildingMalus = -10;
    this.d2dTerrainBonus = 0;

    this.d2dCoZoneTerrainBonus = 2;
    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.getTerrainDefenseModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (CO.isActive(co))
            {
                if (CO_CAIRN.isWildernessTile(posX, posY, map))
                {
                    switch (co.getPowerMode())
                    {
                    case GameEnums.PowerMode_Tagpower:
                    case GameEnums.PowerMode_Superpower:
                    case GameEnums.PowerMode_Power:
                        return CO_CAIRN.powerTerrainBonus;
                    default:
                        if (co.inCORange(Qt.point(posX, posY), unit))
                        {
                            return CO_CAIRN.d2dCoZoneTerrainBonus;
                        }
                        else
                        {
                            return CO_CAIRN.d2dTerrainBonus;
                        }
                    }
                }
            }
        }
        return 0;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (map !== null)
            {
                if (map.onMap(atkPosX, atkPosY))
                {
                    var terrain = map.getTerrain(atkPosX, atkPosY);
                    var startpower = 0;
                    if (terrain.getBuilding() !== null)
                    {
                        startpower = CO_CAIRN.d2dBuildingMalus;
                    }
                    switch (co.getPowerMode())
                    {
                    case GameEnums.PowerMode_Tagpower:
                    case GameEnums.PowerMode_Superpower:
                        if (CO_CAIRN.isWildernessTile(atkPosX, atkPosY, map))
                        {
                            var terrainDefense = terrain.getDefense(attacker);
                            return terrainDefense * CO_CAIRN.superPowerOffBonus + CO_CAIRN.powerOffBonus;
                        }
                        else
                        {
                            return CO_CAIRN.powerOffBonus + startpower;
                        }
                    case GameEnums.PowerMode_Power:
                        return CO_CAIRN.powerOffBonus + startpower;
                    default:
                        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                        {
                            return CO_CAIRN.d2dCoZoneOffBonus + startpower;
                        }
                        else
                        {
                            return startpower;
                        }
                    }
                }
            }
        }
        return 0;
    };
    this.getVisionrangeModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (CO_CAIRN.isWildernessTile(posX, posY, map))
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    break;
                case GameEnums.PowerMode_Power:

                    if (unit.getBaseMaxRange() > 1)
                    {
                        return CO_CAIRN.powerVisionrangeModifier;
                    }
                    break;
                default:
                    break;
                }
            }
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (CO_CAIRN.isWildernessTile(posX, posY, map))
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    break;
                case GameEnums.PowerMode_Power:

                    if (unit.getBaseMaxRange() > 1)
                    {
                        return CO_CAIRN.powerFirerangeBonus;
                    }
                    break;
                default:
                    break;
                }
            }
        }
        return 0;
    };
    this.getDeffensiveReduction = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isAttacker, action, luckMode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (CO_CAIRN.isWildernessTile(defPosX, defPosY, map))
                {
                    var terrainDefense = map.getTerrain(defPosX, defPosY).getDefense(defender);
                    return terrainDefense * CO_CAIRN.superPowerDefenseReduction;
                }
                break;
            case GameEnums.PowerMode_Power:
                break;
            default:
                break;
            }
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_CAIRN.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_CAIRN.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getMovementcostModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (unit.getOwner() === co.getOwner())
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    break;
                case GameEnums.PowerMode_Power:
                    if (CO_CAIRN.isWildernessTile(posX, posY, map))
                    {
                        return CO_CAIRN.powerMovementCostReduction;
                    }
                    break;
                default:
                }
            }
        }
        return 0;
    };
    this.postAction = function(co, action, map)
    {
        if (CO.isActive(co))
        {
            if (CO.isActive(co))
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    var unit = action.getPerformingUnit();
                    if (unit !== null && unit.getHp() > 0)
                    {
                        var path = action.getMovePath();
                        var heal = 0;
                        for (var i = 0; i < path.length; ++i)
                        {
                            var pos = path[i];
                            if (CO_CAIRN.isWildernessTile(pos.x, pos.y, map))
                            {
                                heal += CO_CAIRN.superPowerHeal;
                            }
                        }
                        if (heal > 0)
                        {
                            unit.setHp(unit.getHpRounded() + heal);
                        }
                    }
                    break;
                case GameEnums.PowerMode_Power:
                    break;
                default:
                    break;
                }
            }
        }
    };

    this.getCOUnits = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            var buildingId = building.getBuildingID();
            if (buildingId === "FACTORY" ||
                    buildingId === "TOWN" ||
                    buildingId === "HQ" ||
                    buildingId === "FORTHQ")
            {
                return ["ZCOUNIT_RANGER"];
            }
        }
        return [];
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A wilderness survival expert who excels at using natural terrain to her advantage.");
    };
    this.getHits = function(co)
    {
        return qsTr("Hiking");
    };
    this.getMiss = function(co)
    {
        return qsTr("Crowds");
    };
    this.getCODescription = function(co)
    {
        var text = qsTr("Wilderness grant her units additional terrain stars. On-property firepower of her units is reduced.");
        return text;
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nRanger\n") +
                   qsTr("\nGlobal Effect: \nWilderness grant her units additional %0 terrain stars. On-property firepower of her units is reduced by %1%.") +
                   qsTr("\n\nCO Zone Effect: \nWilderness grant her units additional %0 terrain stars. On-property firepower of her units is reduced by %1%.");
        text = replaceTextArgs(text, [CO_CAIRN.d2dTerrainBonus, CO_CAIRN.d2dBuildingMalus, CO_CAIRN.d2dCoZoneTerrainBonus, CO_CAIRN.d2dBuildingMalus]);
        return text;
    };

    this.getPowerDescription = function(co)
    {
        var text = qsTr("Wilderness movement cost is reduced by %0. Unit indirect range is increased by %1 and vision is increased by %2 in wilderness.");
        text = replaceTextArgs(text, [CO_CAIRN.powerMovementCostReduction, CO_CAIRN.powerFirerangeBonus, CO_CAIRN.powerVisionrangeModifier]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Call of the Wild");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Each wilderness tile crossed restores %0 HP. Increases her firepower by %0% and lower enemy defense by %1% for each wilderness terrain star.");
        text = replaceTextArgs(text, [CO_CAIRN.superPowerHeal, CO_CAIRN.superPowerOffBonus, CO_CAIRN.superPowerDefenseReduction]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Force of Nature");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("The flow of battle favors this moment."),];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Nature enpowers the wise."),
                qsTr("Walk where others fail to tread."),
                qsTr("Follow the path that naturally leads to victory."),
                qsTr("Tranquility restored."),
                qsTr("All is well."),];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("A humbling journey to reflect on."),];
    };
    this.getName = function()
    {
        return qsTr("Cairn");
    };

    this.showDefaultUnitGlobalBoost = function(co)
    {
        return false;
    };
    this.getCustomUnitGlobalBoostCount = function(co)
    {
        return terrainSpriteManager.getTerrainCount() +
               buildingSpriteManager.getBuildingCount();
    };
    this.getCustomUnitGlobalBoost = function(co, index, info, map)
    {
        var terrains = terrainSpriteManager.getTerrainsSorted();
        var buildings = buildingSpriteManager.getLoadedBuildings();
        var id = "";
        var wilderness = false;
        var building = false;
        if (index < terrains.length)
        {
            id = terrains[index];
            wilderness = CO_CAIRN.wilderness.includes(id);
        }
        else
        {
            id = buildings[index - terrains.length];
            building = true;
        }
        info.setIconId(id);
        info.setLink(id);
        var defenseStars = Global[id].getDefense(null);
        var powerMode = co.getPowerMode();
        var defenseBoost = 0;
        var offensiveBoost = 0;
        if (powerMode === GameEnums.PowerMode_Tagpower ||
            powerMode === GameEnums.PowerMode_Superpower)
        {
            defenseBoost = CO_CAIRN.powerDefBonus;
            if (wilderness)
            {
                info.addBonusIcon("defenseStar", "+" + CO_CAIRN.powerTerrainBonus);
                offensiveBoost = CO_CAIRN.superPowerOffBonus * (defenseStars + CO_CAIRN.powerTerrainBonus) + CO_CAIRN.powerOffBonus;
            }
            else if (building)
            {
                offensiveBoost = CO_CAIRN.d2dBuildingMalus + CO_CAIRN.powerOffBonus;
            }
            else
            {
                offensiveBoost = CO_CAIRN.powerOffBonus;
            }
        }
        else if (powerMode === GameEnums.PowerMode_Power)
        {
            defenseBoost = CO_CAIRN.powerDefBonus;
            if (wilderness)
            {
                info.addBonusIcon("moveRange", CO_CAIRN.powerMovementCostReduction.toString());
                info.addBonusIcon("atkRange", "+" + CO_CAIRN.powerFirerangeBonus.toString());
            }
        }
        else
        {
            defenseBoost = 0;
            offensiveBoost = 0;
            if (wilderness)
            {
                info.addBonusIcon("defenseStar", "+" + CO_CAIRN.d2dTerrainBonus);
            }
            else if (building)
            {
                offensiveBoost = CO_CAIRN.d2dBuildingMalus;
            }
        }
        info.setOffensiveBoost(offensiveBoost);
        info.setDefensiveBoost(defenseBoost);
    };
    this.showDefaultUnitZoneBoost = function(co)
    {
        return false;
    };
    this.getCustomUnitZoneBoostCount = function(co)
    {
        return terrainSpriteManager.getTerrainCount() +
               buildingSpriteManager.getBuildingCount();
    };
    this.getCustomUnitZoneBoost = function(co, index, info, map)
    {
        var terrains = terrainSpriteManager.getTerrainsSorted();
        var buildings = buildingSpriteManager.getLoadedBuildings();
        var id = "";
        var wilderness = false;
        var building = false;
        if (index < terrains.length)
        {
            id = terrains[index];
            wilderness = CO_CAIRN.wilderness.includes(id);
        }
        else
        {
            id = buildings[index - terrains.length];
            building = true;
        }
        info.setIconId(id);
        info.setLink(id);
        var defenseStars = Global[id].getDefense(null);
        var powerMode = co.getPowerMode();
        var defenseBoost = 0;
        var offensiveBoost = 0;
        if (powerMode === GameEnums.PowerMode_Tagpower ||
            powerMode === GameEnums.PowerMode_Superpower)
        {
            defenseBoost = CO_CAIRN.powerDefBonus;
            if (wilderness)
            {
                info.addBonusIcon("defenseStar", "+" + CO_CAIRN.powerTerrainBonus);
                offensiveBoost = CO_CAIRN.superPowerOffBonus * (defenseStars + CO_CAIRN.powerTerrainBonus) + CO_CAIRN.powerOffBonus;
            }
            else if (building)
            {
                offensiveBoost = CO_CAIRN.d2dBuildingMalus + CO_CAIRN.powerOffBonus;
            }
            else
            {
                offensiveBoost = CO_CAIRN.powerOffBonus;
            }
        }
        else if (powerMode === GameEnums.PowerMode_Power)
        {
            defenseBoost = CO_CAIRN.powerDefBonus;
            if (wilderness)
            {
                info.addBonusIcon("moveRange", CO_CAIRN.powerMovementCostReduction.toString());
                info.addBonusIcon("atkRange", "+" + CO_CAIRN.powerFirerangeBonus.toString());
            }
        }
        else
        {
            defenseBoost = CO_CAIRN.d2dCoZoneOffBonus;
            offensiveBoost = CO_CAIRN.d2dCoZoneDefBonus;
            if (wilderness)
            {
                info.addBonusIcon("defenseStar", "+" + CO_CAIRN.d2dCoZoneTerrainBonus);
            }
            else if (building)
            {
                offensiveBoost = CO_CAIRN.d2dCoZoneOffBonus + CO_CAIRN.d2dBuildingMalus;
            }
        }
        info.setOffensiveBoost(offensiveBoost);
        info.setDefensiveBoost(defenseBoost);
    };
}

Constructor.prototype = CO;
var CO_CAIRN = new Constructor();
