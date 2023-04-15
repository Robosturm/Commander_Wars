// this is the base class for terrain
var BUILDING =
{
    // search type filter for overlay
    getBuildingGroup : function()
    {
        return 0;
    },
    getBuildingGroupSort : function()
    {
        return [0,
                1,
                2,
                3,
                4,
                5,
                6,];

    },
    buildingGroupNameMapping = [qsTr("Production"),     // 0
                                qsTr("Income"),         // 1
                                qsTr("Utility"),        // 2
                                qsTr("Black Hole"),     // 3
                                qsTr("Bolt Guard"),     // 4
                                qsTr("Dark Matter"),    // 5
                                qsTr("Other")],         // 6

    getBuildingGroupName : function(item)
    {
        if (item >= 0 && item < buildingGroupNameMapping.length)
        {
            return buildingGroupNameMapping[item];
        }
        return qsTr("Unknown");
    },

    // loader for stuff which needs C++ Support
    init : function (building, map)
    {
        building.setVisionHigh(1);
    },

    getBuildingBaseTerrain : function(building, map)
    {
        var terrain = building.getTerrain();
        var baseId = "plains";
        if (terrain !== null)
        {
            baseId = terrain.getBaseTerrainID().toLowerCase();
        }
        return baseId;
    },

    getName : function(building, map)
    {
        return "";
    },
    // returns the defense of this terrain
    getDefense : function(building, map)
    {
        return 3;
    },

    // vision bonus for units created by this building
    getVisionBonus : function(building, map)
    {
        return 0;
    },
    // additional offensive bonus for a unit on this field
    getOffensiveFieldBonus : function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckMode, map)
    {
        return 0;
    },
    //  additional deffensive bonus for a unit on this field
    getDeffensiveFieldBonus : function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action, luckMode, map)
    {
        return 0;
    },
    getBuildingWidth : function(map)
    {
        // one field width default for most buildings
        return 1;
    },
    getBuildingHeigth : function(map)
    {
        // one field heigth default for most buildings
        return 1;
    },
    loadSprites : function(building, neutral, map)
    {
    },
    // the terrain on which a building can be placed
    // if the current terrain isn't in the list. it'll be replaced by the first :)
    baseTerrains : ["PLAINS", "STREET", "STREET1", "SNOW", "SNOW_STREET", "DESERT", "DESERT_PATH", "DESERT_PATH1", "WASTE", "WASTE_PATH1"],
    getBaseTerrain : function(building, map)
    {
        return Global[building.getBuildingID()].baseTerrains;
    },

    addCaptureAnimationBuilding : function(animation, building, startPlayer, capturedPlayer, map)
    {
        animation.addBuildingSprite("town+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Matrix);
    },

    canLargeBuildingPlaced : function(terrain, building, width, heigth, map)
    {
        var placeX = terrain.getX();
        var placeY = terrain.getY();
        var baseTerrains = building.getBaseTerrain();
        for (var x = 0; x < width; x++)
        {
            for (var y = 0; y < heigth; y++)
            {
                if (map.onMap(placeX - x, placeY - y))
                {
                    if (baseTerrains.indexOf(map.getTerrain(placeX - x, placeY - y).getTerrainID()) < 0)
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
        return true;
    },

    canBuildingBePlaced : function(terrain, building, map)
    {
        var baseTerrains = building.getBaseTerrain();
        if (baseTerrains.indexOf(terrain.getTerrainID()) >= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    },

    getBaseIncome : function(building, map)
    {
        return 1000;
    },

    getCostModifier : function(building, unitId, baseCost, posX, posY, map)
    {
        return 0;
    },

    getConstructionList : function(building, map)
    {
        return [];
    },
    actionList : [],
    getActions : function(building, map)
    {
        return Global[building.getBuildingID()].actionList;
    },

    endOfTurn : function(building, map)
    {
        // gets called at the end of a turn
    },

    startOfTurn : function(building, map)
    {
        var owner = building.getOwner();
        if (owner !== null)
        {
            if (!owner.getIsDefeated())
            {
                BUILDING.replenishUnit(building, map);
            }
        }
    },

    getOffensiveBonus : function(building, map)
    {
        return 0;
    },

    getDefensiveBonus : function(building, map)
    {
        return 0;
    },
    getPowerChargeBonus : function(building, map)
    {
        return 0;
    },
    getActionTargetFields : function(building, map)
    {
        // targets of a building. For most things this is a null pointer
        // return must be null or a QmlVectorPoint
        return null;
    },
    getActionTargetOffset : function(building, map)
    {
        // offset for large buildings since there reference point is bound to the lower right corner.
        return Qt.point(0, 0);
    },
    getIsAttackable : function(building, x, y, map)
    {
        return true;
    },

    getRepairTypes : function(building, map)
    {
        return[];
    },

    replenishUnit : function(building, map, health = 2, fuelAmount = 1, ammo1Amount = 1, ammo2Amount = 1, always = false)
    {
        // default impl replenishes our units
        // gets called at the start of a turn
        var constructionList = building.getConstructionList();
        var repairList = building.getRepairTypes();
        var unit = building.getTerrain().getUnit();
        if ((unit !== null) &&
            (unit.getOwner() === building.getOwner() || always) &&
            ((repairList.indexOf(unit.getUnitType()) >= 0) ||
             (constructionList.indexOf(unit.getUnitID()) >= 0)))
        {
            var x = unit.getX();
            var y = unit.getY();
            if (unit.canBeRepaired(Qt.point(x, y)))
            {
                BUILDING.repairUnit(unit, x, y, map, health, fuelAmount, ammo1Amount, ammo2Amount);
            }
        }
    },

    repairUnit : function(unit, x, y, map, health = 2, fuelAmount = 1, ammo1Amount = 1, ammo2Amount = 1)
    {
        // our unit and a repairable one
        // replenish it
        var refillRule = map.getGameRules().getGameRule("GAMERULE_REFILL_MATERIAL");
        var refillMaterial = (typeof refillRule === 'undefined' || refillRule === null); // an existing rule equals it's set
        unit.refill(refillMaterial, fuelAmount, ammo1Amount, ammo2Amount);
        var repairAmount = 0;
        if (health !== 0)
        {
            repairAmount = health + unit.getRepairBonus(Qt.point(x, y));
        }
        UNIT.repairUnit(unit, repairAmount, map);
        if (!unit.isStealthed(map.getCurrentViewPlayer()))
        {
            var animationCount = GameAnimationFactory.getAnimationCount();
            var animation = GameAnimationFactory.createAnimation(map, x, y);
            var width = 0;
            if (repairAmount > 0)
            {
                width = animation.addText(qsTr("REPAIR"), map.getImageSize() / 2 + 25, -2, 1);
            }
            else if (repairAmount < 0)
            {
                width = animation.addText(qsTr("DAMAGE"), map.getImageSize() / 2 + 25, -2, 1);
            }
            else
            {
                width = animation.addText(qsTr("RATION"), map.getImageSize() / 2 + 25, -2, 1);
            }

            animation.addBox("info", map.getImageSize() / 2, 0, width + 36, map.getImageSize(), 400);
            animation.addSprite("repair", map.getImageSize() / 2 + 4, 4, 400, 2);
            animation.addSound("repair_2.wav");
            if (animationCount > 0)
            {
                GameAnimationFactory.getAnimation(animationCount - 1).queueAnimation(animation);
            }
        }
    },

    getMiniMapIcon : function(building, map)
    {
        return "minimap_building";
    },

    onDestroyed : function(building, map)
    {
        // called when the building is destroyed and replacing of this building starts
    },

    getDamage : function(building, unit, map)
    {
        return 0;
    },

    getBuildingTargets : function(building, map)
    {
        // hint for the ai
        return GameEnums.BuildingTarget_Own;
    },

    getTerrainAnimationMoveSpeed : function(map)
    {
        return 0;
    },

    getTerrainAnimationBase : function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        return "base_" + weatherModifier + "air";
    },

    getTerrainAnimationForeground : function(unit, terrain, defender, map)
    {
        return "";
    },


    armyData = [["ac", "yc"],
                ["os", "os"],
                ["bm", "bm"],
                ["ge", "ge"],
                ["yc", "yc"],
                ["gs", "yc"],
                ["ti", "ge"],
                ["dm", "ge"],
                ["pf", "os"],
                ["bd", "bm"],],

    getTerrainAnimationBackground : function(unit, terrain, defender, map)
    {
        var variables = terrain.getAnimationVariables();
        var variable = variables.getVariable("BACKGROUND_ID");
        var armyVariable = variables.getVariable("ARMYBACKGROUND_ID");
        var rand = 0;
        var randArmy = 0;
        if (variable === null)
        {
            rand = globals.randInt(0, 1, true);
            variable = variables.createVariable("BACKGROUND_ID");
            armyVariable = variables.createVariable("ARMYBACKGROUND_ID");
            variable.writeDataInt32(rand);
            randArmy = globals.randInt(0, BUILDING.armyData.length - 1);
            armyVariable.writeDataInt32(randArmy);
        }
        else
        {
            rand = variable.readDataInt32();
            randArmy = armyVariable.readDataInt32();
        }
        var building = terrain.getBuilding();
        var player = building.getOwner();

        var army = BUILDING.armyData[randArmy][1];
        if (player !== null)
        {
            army = Global.getArmyNameFromPlayerTable(player, BUILDING.armyData);
        }
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        if (weatherModifier === "")
        {
            weatherModifier = TERRAIN.getTerrainWeatherModifier(terrain);
        }

        if (weatherModifier === "desert")
        {
            return "back_deserttown";
        }
        else if (weatherModifier === "waste")
        {
            return "back_wastetown";
        }
        return "back_" + weatherModifier + "town+" + army + "+" + rand.toString();
    },

    getDescription : function(building)
    {
        return "";
    },

    // vision created by this field
    getVision : function(building)
    {
        return 0;
    },

    getVisionHide : function(building)
    {
        return false;
    },

    onCaptured : function(building)
    {
    },

    onWeatherChanged : function(building, weather)
    {
        // called when the weather changes
        // call loadWeatherOverlaySpriteV2 to load an sprite overlay
    },

    getShowInEditor : function()
    {
        return true;
    },

    usesMapLayer : function()
    {
        return false;
    },

    getCostReduction : function(building, unitId, baseCost, x, y, map)
    {
        return 0;
    },

    getEditorPlacementSound : function()
    {
        return "placeBuilding.wav";
    },

    getBuildingsSorted : function()
    {

    }
};
