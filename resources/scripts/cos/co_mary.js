var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation = GameAnimationFactory.createAnimation(map, 0, 0);
        animation.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation);
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
                audio.addMusic("resources/music/cos/mary.mp3", 8299, 73941);
                break;
            }
        }
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co) && defender !== null)
        {
            var destroyed = false;
            var x = -1;
            var y = -1;
            var unitID = 0;
            if (gotAttacked && defender.getOwner() === co.getOwner())
            {
                if (attacker.getHp() <= 0)
                {
                    destroyed = true;
                    x = attacker.getX();
                    y = attacker.getY();
                }
                else if (attacker.getOwner() === co.getOwner())
                {
                    unitID = attacker.getUniqueID();
                }
            }
            else
            {
                if (defender.getHp() <= 0)
                {
                    destroyed = true;
                    x = defender.getX();
                    y = defender.getY();
                }
                else
                {
                    unitID = defender.getUniqueID();
                }
            }
            // store variable data

            var variables = co.getVariables();
            if (destroyed)
            {
                var building = map.getTerrain(x, y).getBuilding();
                if (building !== null)
                {
                    // store capture bonus
                    var xVariable = variables.createVariable("POSXBUILDINGS");
                    var yVariable = variables.createVariable("POSYBUILDINGS");
                    var pointsX = xVariable.readDataListInt32();
                    var pointsY = yVariable.readDataListInt32();
                    var found = false;
                    for (var i = 0; i < pointsX.length; i++)
                    {
                        if (pointsX[i] === x &&
                                pointsY[i] === y)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found === false)
                    {
                        pointsX.push(x);
                        pointsY.push(y);
                        xVariable.writeDataListInt32(pointsX);
                        yVariable.writeDataListInt32(pointsY);
                    }
                }
            }
            else
            {
                // store repair block
                var repairVariable = variables.createVariable("UNITREPAIR");
                var repairs = repairVariable.readDataListInt32();
                repairs.push(unitID);
                repairVariable.writeDataListInt32(repairs);
            }
        }
    };

    this.startOfTurn = function(co, map)
    {
        // reset repair stop
        var variables = co.getVariables();
        var repairVariable = variables.createVariable("UNITREPAIR");
        var empty = [];
        repairVariable.writeDataListInt32(empty);
    };

    this.canBeRepaired = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            // disable enemy repairs
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                return false;
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            {
                var variables = co.getVariables();
                var repairVariable = variables.createVariable("UNITREPAIR");
                var repairs = repairVariable.readDataListInt32();
                var unitId = unit.getUniqueID();
                if (repairs.indexOf(unitId) >= 0)
                {
                    return false;
                }
                break;
            }
            default:
                if (map === null ||
                    (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    var variables = co.getVariables();
                    var repairVariable = variables.createVariable("UNITREPAIR");
                    var repairs = repairVariable.readDataListInt32();
                    var unitId = unit.getUniqueID();
                    if (repairs.indexOf(unitId) >= 0)
                    {
                        return false;
                    }
                }
                break;
            }
        }
        return true;
    };

    this.superPowerOffBonus = 30;
    this.superPowerDefenseReduction = 15;
    this.superPowerCaptureBonus = 15;
    this.powerOffBonus = 20;
    this.powerDefBonus = 10;

    this.d2dCoZoneOffBonus = 20;
    this.d2dCoZoneDefBonus = 10;

    this.d2dCaptureBonus = 5;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_MARY.superPowerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_MARY.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_MARY.d2dCoZoneOffBonus;
                }
                return 0;
            }
        }
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_MARY.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_MARY.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getDeffensiveReduction = function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                if (co.getOwner().isEnemyUnit(defender) && defender.useTerrainDefense())
                {
                    return defender.getTerrainDefense() * CO_MARY.superPowerDefenseReduction;
                }
            }
        }
        return 0;
    };

    this.getAdditionalBuildingActions = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return ""
            case GameEnums.PowerMode_Power:
                // disable enemy production line
                if (co.getOwner().isEnemy(building.getOwner()))
                {
                    return "-ACTION_BUILD_UNITS";
                }
                break;
            default:
                break;
            }
        }
        return "";
    };

    this.getCaptureBonus = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            var applyBonus = false;
            // store capture bonus
            var variables = co.getVariables();
            var xVariable = variables.createVariable("POSXBUILDINGS");
            var yVariable = variables.createVariable("POSYBUILDINGS");
            var pointsX = xVariable.readDataListInt32();
            var pointsY = yVariable.readDataListInt32();
            for (var i = 0; i < pointsX.length; i++)
            {
                if (pointsX[i] === posX &&
                        pointsY[i] === posY)
                {
                    // apply capture bonus and remove it from the list
                    applyBonus = true;
                    pointsX.splice(i, 1);
                    pointsY.splice(i, 1);
                    xVariable.writeDataListInt32(pointsX);
                    yVariable.writeDataListInt32(pointsY);
                    break;
                }
            }
            var powerMode = co.getPowerMode();
            if ((powerMode === GameEnums.PowerMode_Superpower ||
                 powerMode === GameEnums.PowerMode_Tagpower) &&
                    applyBonus === true)
            {
                return CO_MARY.superPowerCaptureBonus;
            }
            else if (applyBonus === true)
            {
                if (powerMode === GameEnums.PowerMode_Power)
                {
                    return CO_MARY.d2dCaptureBonus;
                }
                else if (map === null ||
                    (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_MARY.d2dCaptureBonus;
                }
            }
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    this.getCOUnitRange = function(co, map)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };
    this.getCOUnits = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            var buildingId = building.getBuildingID();
            if (buildingId === "FACTORY" ||
                    buildingId === "TOWN" ||
                    BUILDING.isHq(building))
            {
                return ["ZCOUNIT_AT_CYCLE"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A ruthless commander recruited into the Amber Corona Army.");
    };
    this.getHits = function(co)
    {
        return qsTr("Cocktails");
    };
    this.getMiss = function(co)
    {
        return qsTr("Excuses");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Attacked enemy units won't repair from buildings for one turn. Whenever Mary destroys a unit on a property, she gains a one-time capture bonus for that property.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_MARY.d2dCaptureBonus * 10];
        }
        var text = qsTr("\nSpecial Unit:\nAT Cycle\n") +
               qsTr("\nGlobal Effect: \nAttacked enemy units won't repair from buildings for one turn. Whenever Mary destroys a unit on a property, she gains a one-time capture bonus of +%0% for that property.") +
               qsTr("\n\nCO Zone Effect: \nMary's units gain +%1% firepower and +%2% defence.");
        text = replaceTextArgs(text, [values[0], CO_MARY.d2dCoZoneOffBonus, CO_MARY.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Enemy units cannot repair from properties on their next turn and enemies cannot deploy any units from properties. Mary's units gain +%0% firepower and +%1% defence.");
        text = replaceTextArgs(text, [CO_MARY.powerOffBonus, CO_MARY.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Highway of Pain");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Terrain stars will reduce enemy defence by -%2% per star. Mary's one-time capture bonus after destroying a unit on a property is increased to +%0% for this turn. Her units gain +%1% firepower and +%3% defence.");
        text = replaceTextArgs(text, [CO_MARY.superPowerCaptureBonus*10,CO_MARY.superPowerOffBonus, CO_MARY.superPowerDefenseReduction, CO_MARY.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Bloody Mary");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Keep marching on, soldiers, victory is close!"),
                qsTr("No pain, NO GLORY!"),
                qsTr("A drop of blood is a drop of effort... You must be proud of your troops."),
                qsTr("People call me Bloody Mary.... and no, you can't drink me."),
                qsTr("We will fight until every single one of your troops are no more!"),
                qsTr("I am a soldier. I fight where I am told, and I win where I fight!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Hah! Looks like all my effort paid off in the end."),
                qsTr("Let's go, people. Let's have a drink to celebrate this victory."),
                qsTr("All right troops, you can rest now. Your job here is done.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("You were better than me... this time."),
                qsTr("There is no way I actually lost. I need a drink now.")];
    };
    this.getName = function()
    {
        return qsTr("Mary");
    };
}

Constructor.prototype = CO;
var CO_MARY = new Constructor();
