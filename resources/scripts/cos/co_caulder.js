var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(7);
        co.setSuperpowerStars(5);
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        if (turnMode === GameEnums.AiTurnMode_StartOfDay)
        {
            if (co.canUseSuperpower())
            {
                return GameEnums.PowerMode_Superpower;
            }
            else if (powerSurplus <= 0.5 &&
                     co.canUsePower())
            {
                return CO.getAiUsePowerAtUnitCount(co, powerSurplus, turnMode, repairUnits);
            }
        }
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
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(CO_CAULDER.powerHeal);
            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power9_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power9_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(CO_CAULDER.superPowerHeal);
            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            animation.setSound("power11.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power11", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power11", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0, 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0, 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/caulder.mp3", 6755, 60471)
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
        return "DM";
    };

    this.superPowerHeal = 10;
    this.superPowerBonus = 60;

    this.powerHeal = 5;
    this.powerBonus = 60;

    this.d2dHealing = 0;
    this.d2dOffBonus = 0;
    this.d2dDefBonus = 0;

    this.d2dCoZoneOffBonus = 60;
    this.d2dCoZoneDefBonus = 60;
    this.d2dCoZoneHealing = 5;

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_CAULDER.superPowerBonus;
            case GameEnums.PowerMode_Power:
                return CO_CAULDER.powerBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_CAULDER.d2dCoZoneOffBonus;
                }
                else
                {
                    return CO_CAULDER.d2dOffBonus;
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
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_CAULDER.superPowerBonus;
            case GameEnums.PowerMode_Power:
                return CO_CAULDER.powerBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_CAULDER.d2dCoZoneDefBonus;
                }
                else
                {
                    return CO_CAULDER.d2dDefBonus;
                }
            }
        }
    };

    this.startOfTurn = function(co, map)
    {
        if (CO.isActive(co))
        {
            var player = co.getOwner();
            if (!player.getIsDefeated())
            {
                var counit = co.getCOUnit();
                var coRange = co.getCORange();
                var animations = [];
                var animation = null;
                var counter = 0;
                var viewplayer = map.getCurrentViewPlayer();
                var size = 0;
                var delay = 0;
                var unit = null;
                if (counit !== null && CO_CAULDER.d2dCoZoneHealing > 0)
                {
                    UNIT.repairUnit(counit, CO_CAULDER.d2dCoZoneHealing, map);
                    var fields = globals.getCircle(1, coRange);
                    var x = counit.getX();
                    var y = counit.getY();
                    size = fields.size();
                    for (var i = 0; i < size; i++)
                    {
                        var point = fields.at(i);
                        var unitX = x + point.x;
                        var unitY = y + point.y;
                        if (map.onMap(unitX, unitY))
                        {
                            unit = map.getTerrain(unitX, unitY).getUnit();
                            if ((unit !== null) &&
                                    (unit.getOwner() === counit.getOwner()))
                            {
                                UNIT.repairUnit(unit, CO_CAULDER.d2dCoZoneHealing, map);
                                delay = globals.randInt(135, 265);
                                if (animations.length < 5)
                                {
                                    delay *= i;
                                }
                                animation = GameAnimationFactory.createAnimation(map, unitX, unitY);
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
                if (CO_CAULDER.d2dHealing)
                {
                    var units = co.getOwner().getUnits();
                    units.randomize();
                    size = units.size();
                    for (var i = 0; i < size; i++)
                    {
                        unit = units.at(i);
                        UNIT.repairUnit(unit, CO_CAULDER.d2dHealing, map);
                        delay = globals.randInt(135, 265);
                        if (animations.length < 5)
                        {
                            delay *= i;
                        }
                        animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
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
                        if (!viewplayer.getFieldVisible(unit.getX(), unit.getY()))
                        {
                            animation.setVisible(false);
                        }
                    }
                }
            }
        }
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
                return ["ZCOUNIT_CRYSTAL_TANK"];
            }
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Head of IDS - the research department of Dark Matter. Conducts in inhuman experiments. All he wants is to be free to satisfy his curiosity.");
    };
    this.getHits = function(co)
    {
        return qsTr("Unrestricted experiments");
    };
    this.getMiss = function(co)
    {
        return qsTr("Ethics");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Caulder can reinforce his army while he is on the field.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nCrystal Tanks\n") +
                   qsTr("\nGlobal Effect: \nUnits have %0% firepower and %1% defense. They also heal %2 HP each turn.") +
                   qsTr("\n\nCO Zone Effect: \nUnits gain %3% firepower and %4% defense. They also heal %5HP each turn.");
        text = replaceTextArgs(text, [CO_CAULDER.d2dOffBonus, CO_CAULDER.d2dDefBonus, CO_CAULDER.d2dHealing,
                                      CO_CAULDER.d2dCoZoneOffBonus, CO_CAULDER.d2dCoZoneDefBonus, CO_CAULDER.d2dCoZoneHealing]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("All of his units gain %0 HP and have %1% increased firepower and defense.");
        text = replaceTextArgs(text, [CO_CAULDER.powerHeal, CO_CAULDER.powerBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Mass Regeneration");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("All his units heal %0 HP while gaining %1% increased firepower and defense.");
        text = replaceTextArgs(text, [CO_CAULDER.superPowerHeal, CO_CAULDER.superPowerBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Perfect Healing");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Your silly moral values have no place on the battlefield."),
                qsTr("You are a worthy specimen for further research."),
                qsTr("Your ethics make you weak. Allow me to demonstrate."),
                qsTr("I am curious to see if you can survive this.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Interesting. Very interesting."),
                qsTr("Quite satisfactory."),
                qsTr("Fascinating...")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I may have to use the Great Owl after all."),
                qsTr("What useless clones. I will have to produce better ones.")];
    };
    this.getName = function()
    {
        return qsTr("Caulder");
    };
}

Constructor.prototype = CO;
var CO_CAULDER = new Constructor();
