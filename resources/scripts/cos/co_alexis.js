var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(4);
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

        var animations = [];
        var counter = 0;
        var buildings = co.getOwner().getBuildings();
        buildings.randomize();
        var fields = globals.getCircle(0, CO_ALEXIS.powerRadius);
        for (var i2 = 0; i2 < buildings.size(); i2++)
        {
            var building = buildings.at(i2);
            var x = building.getX();
            var y = building.getY();
            var animation = null;
            for (var i = 0; i < fields.size(); i++)
            {
                var point = fields.at(i);
                if (map.onMap(x + point.x, y + point.y))
                {
                    var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                    if ((unit !== null) &&
                            (unit.getOwner() === co.getOwner()))
                    {
                        animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                        animation.writeDataInt32(unit.getX());
                        animation.writeDataInt32(unit.getY());
                        animation.writeDataInt32(CO_ALEXIS.powerHeal);
                        animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
                        var delay = globals.randInt(135, 265);
                        if (animations.length < 5)
                        {
                            delay *= i;
                        }
                        animation.setSound("power0.wav", 1, delay);
                        if (animations.length < 5)
                        {
                            animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                            powerNameAnimation.queueAnimation(animation);
                            animations.push(animation);
                        }
                        else
                        {
                            animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                            animations[counter].queueAnimation(animation);
                            animations[counter] = animation;
                            counter++;
                            if (counter >= animations.length)
                            {
                                counter = 0;
                            }
                        }

                    }
                }
            }
        }
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var animations = [];
        var counter = 0;
        var player = co.getOwner();
        var buildings = player.getBuildings();
        buildings.randomize();
        var fields = globals.getCircle(0, CO_ALEXIS.superPowerHealRadius);

        var unit =  null;
        var building = null;
        var animation = null;
        var x = 0;
        var y = 0;
        var i = 0;
        var i2 = 0;
        for (i2 = 0; i2 < buildings.size(); i2++)
        {
            building = buildings.at(i2);
            x = building.getX();
            y = building.getY();
            animation = null;
            for (i = 0; i < fields.size(); i++)
            {
                var point = fields.at(i);
                if (map.onMap(x + point.x, y + point.y))
                {
                    var unitX = x + point.x;
                    var unitY = y + point.y;
                    unit = map.getTerrain(unitX, unitY).getUnit();
                    if ((unit !== null) &&
                            (unit.getOwner() === player))
                    {
                        animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                        animation.writeDataInt32(unitX);
                        animation.writeDataInt32(unitY);
                        animation.writeDataInt32(CO_ALEXIS.superPowerHeal);
                        animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
                        var delay = globals.randInt(135, 265);
                        if (animations.length < 5)
                        {
                            delay *= i;
                        }
                        animation.setSound("power4.wav", 1, delay);
                        if (animations.length < 5)
                        {
                            animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                            powerNameAnimation.queueAnimation(animation);
                            animations.push(animation);
                        }
                        else
                        {
                            animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                            animations[counter].queueAnimation(animation);
                            animations[counter] = animation;
                            counter++;
                            if (counter >= animations.length)
                            {
                                counter = 0;
                            }
                        }
                    }
                }
            }
        }
        fields = globals.getCircle(0, CO_ALEXIS.superPowerDamageRadius);
        var playerCounter = map.getPlayerCount();
        for (var i3 = 0; i3 < playerCounter; i3++)
        {
            var enemyPlayer = map.getPlayer(i3);
            buildings = enemyPlayer.getBuildings();
            buildings.randomize();
            if ((enemyPlayer !== player) &&
                    (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {
                for (i2 = 0; i2 < buildings.size(); i2++)
                {
                    building = buildings.at(i2);
                    x = building.getX();
                    y = building.getY();
                    animation = null;
                    for (i = 0; i < fields.size(); i++)
                    {
                        point = fields.at(i);
                        if (map.onMap(x + point.x, y + point.y))
                        {
                            unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                            if ((unit !== null) &&
                                    (co.getOwner().isEnemyUnit(unit)))
                            {
                                animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                                animation.writeDataInt32(unit.getX());
                                animation.writeDataInt32(unit.getY());
                                animation.writeDataInt32(CO_ALEXIS.superPowerDamage);
                                animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
                                var delay = globals.randInt(135, 265);
                                if (animations.length < 5)
                                {
                                    delay *= i;
                                }
                                animation.setSound("power4.wav", 1, delay);
                                if (animations.length < 5)
                                {
                                    animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                                    powerNameAnimation.queueAnimation(animation);
                                    animations.push(animation);
                                }
                                else
                                {
                                    animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                                    animations[counter].queueAnimation(animation);
                                    animations[counter] = animation;
                                    counter++;
                                    if (counter >= animations.length)
                                    {
                                        counter = 0;
                                    }
                                }
                            }
                        }
                    }
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
                audio.addMusic("resources/music/cos/alexis.mp3", 51, 56938);
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "PF";
    };
    this.superPowerOffBonus = 50;
    this.superPowerOffBaseBonus = 15;
    this.superPowerHeal = 3;
    this.superPowerHealRadius = 2;
    this.superPowerDamage = 3;
    this.superPowerDamageRadius = 1;

    this.powerOffBonus = 30;
    this.powerOffBaseBonus = 15;
    this.powerDefBonus = 15;
    this.powerHeal = 3;
    this.powerRadius = 2;

    this.d2dCoZoneBonus = 15;

    this.d2dHealBonus = 1;
    this.d2dHealRadius = 1;
    this.d2dHealMalus = 1;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var nearBuildings = false;
            var fields = globals.getCircle(0, 2);
            if (map !== null)
            {
                for (var i = 0; i < fields.size(); i++)
                {
                    var x = fields.at(i).x + atkPosX;
                    var y = fields.at(i).y + atkPosY;
                    if (map.onMap(x, y))
                    {
                        var building = map.getTerrain(x, y).getBuilding();
                        if (building !== null && building.getOwner() === co.getOwner())
                        {
                            nearBuildings = true;
                            break;
                        }
                    }
                }
            }
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (nearBuildings)
                {
                    return CO_ALEXIS.superPowerOffBonus;
                }
                return CO_ALEXIS.superPowerOffBaseBonus;
            case GameEnums.PowerMode_Power:
                if (nearBuildings)
                {
                    return CO_ALEXIS.powerOffBonus;
                }
                return CO_ALEXIS.powerOffBaseBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_ALEXIS.d2dCoZoneBonus;
                }
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
                return CO_ALEXIS.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_ALEXIS.d2dCoZoneBonus;
            }
        }
        return 0;
    };

    this.startOfTurn = function(co, map)
    {
        if (CO.isActive(co))
        {
            var player = co.getOwner();
            if (!player.getIsDefeated())
            {
                var animations = [];
                var counter = 0;
                var buildings = co.getOwner().getBuildings();
                var fields = globals.getCircle(1, CO_ALEXIS.d2dHealRadius);
                var viewplayer = map.getCurrentViewPlayer();
                var size1 = buildings.size();
                var size2 = fields.size();
                for (var i2 = 0; i2 < size1; i2++)
                {
                    var building = buildings.at(i2);
                    var id = building.getBuildingID();
                    if (!id.startsWith("TEMPORARY_"))
                    {
                        var x = building.getX();
                        var y = building.getY();
                        var animation = null;
                        for (var i = 0; i < size2; i++)
                        {
                            var point = fields.at(i);
                            if (map.onMap(x + point.x, y + point.y))
                            {
                                var unitX = x + point.x;
                                var unitY = y + point.y;
                                var unit = map.getTerrain(unitX, unitY).getUnit();
                                if ((unit !== null) &&
                                    (unit.getOwner() === co.getOwner()))
                                {
                                    UNIT.repairUnit(unit, CO_ALEXIS.d2dHealBonus, map);
                                    animation = GameAnimationFactory.createAnimation(map, unitX, unitY);
                                    var delay = globals.randInt(135, 265);
                                    if (animations.length < 5)
                                    {
                                        delay *= i;
                                    }
                                    animation.setSound("power0.wav", 1, delay);
                                    if (animations.length < 5)
                                    {
                                        animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                                        animations.push(animation);
                                    }
                                    else
                                    {
                                        animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                                        animations[counter].queueAnimation(animation);
                                        animations[counter] = animation;
                                        counter++;
                                        if (counter >= animations.length)
                                        {
                                            counter = 0;
                                        }
                                    }
                                    if (!viewplayer.getFieldVisible(unitX, unitY))
                                    {
                                        animation.setVisible(false);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    };

    this.getRepairBonus = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            return -CO_ALEXIS.d2dHealMalus;
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
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
                return ["ZCOUNIT_REPAIR_TANK"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A crystal-obsessed person. Blindly follows orders.");
    };
    this.getHits = function(co)
    {
        return qsTr("Crystals");
    };
    this.getMiss = function(co)
    {
        return qsTr("Experiments");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Units heal at half the normal rate on owned properties, but units still heal while standing next to them.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nRepair Tank\n") +
                   qsTr("\nGlobal Effect: \nUnits heal only %0 HP while on an owned property, however, units will still heal from any owned property within %1 space of a unit by %2 HP. This effect stacks with each additional nearby property.") +
                   qsTr("\n\nCO Zone Effect: \nUnits gain %0% firepower and defence.");
        text = replaceTextArgs(text, [CO_ALEXIS.d2dHealMalus, CO_ALEXIS.d2dHealBonus,  CO_ALEXIS.d2dHealBonus, CO_ALEXIS.d2dCoZoneBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text =  qsTr("Units within %0 spaces of any owned properties receive %1% firepower bonus and restore %2 HP per nearby property. All other units gain %3% firepower and %4% defence.");
        text = replaceTextArgs(text, [CO_ALEXIS.powerRadius, CO_ALEXIS.powerOffBonus,  CO_ALEXIS.powerHeal, CO_ALEXIS.powerOffBaseBonus, CO_ALEXIS.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Crystal Brace");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Units within %0 spaces of any owned properties receive %1% firepower bonuses and restore %2 HP per nearby property. Enemies within %3 spaces of their own properties suffer %4 HP of damage per nearby property. All other units gain %5% firepower and %6% defence.");
        text = replaceTextArgs(text, [CO_ALEXIS.superPowerHealRadius, CO_ALEXIS.superPowerOffBonus,  CO_ALEXIS.superPowerHeal,  CO_ALEXIS.superPowerDamageRadius,  CO_ALEXIS.superPowerDamage, CO_ALEXIS.powerOffBaseBonus, CO_ALEXIS.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Crystal Edge");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("You'll regret angering me!"),
                qsTr("It's time you learned the real power of the crystals!"),
                qsTr("You were probably enjoying this battle... until now!"),
                qsTr("Witness the power of the black crystal!"),
                qsTr("I hope you're ready for what's coming next!"),
                qsTr("Now my army will really shine... like diamonds!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I'm the brightest crystal of all!"),
                qsTr("Haha, you were shattered!"),
                qsTr("I'll leave you to clean this up, 'kay?")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("My crystals failed me."),
                qsTr("My diamonds shattered?")];
    };
    this.getName = function()
    {
        return qsTr("Alexis");
    };
};

Constructor.prototype = CO;
var CO_ALEXIS = new Constructor();
