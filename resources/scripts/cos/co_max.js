var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2", "+alt3"];
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAtUnitCount(co, powerSurplus, turnMode, directUnits);
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
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
            audio.addMusic("resources/music/cos/max.mp3", 57, 70080)
            break;
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
            if (unit.getBaseMaxRange() === 1)
            {
                var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                var delay = globals.randInt(135, 265);
                if (animations.length < 5)
                {
                    delay *= i;
                }
                animation.setSound("power6.wav", 1, delay);
                if (animations.length < 5)
                {
                    animation.addSprite("power6", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power6", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
            if (unit.getBaseMaxRange() === 1)
            {
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
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };

    this.superPowerOffBonus = 70;
    this.superpowerMovementBonus = 2;
    this.powerOffBonus = 45;
    this.powerMovementBonus = 1
    this.powerIndirectOffBonus = 0;
    this.powerOtherBonus = 10;
    this.powerDefBonus = 10;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneOffBonus = 45;
    this.d2dCoZoneIndirectOffBonus = 0;
    this.d2dCoZoneOtherOffBonus = 10;
    this.d2dIndirectFirerangeMalus = -1;
    this.d2dOffBonus = 15;
    this.d2dIndirectOffBonus = -10;
    this.d2dOtherOffBonus = 0;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
     defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var isDirect = (attacker.getBaseMaxRange() === 1 && attacker.getUnitType() !== GameEnums.UnitType_Infantry);
            var isIndirect = (attacker.getBaseMaxRange() > 1);
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                {
                    if (isDirect)
                    {
                        return CO_MAX.superPowerOffBonus;
                    }
                    else if (isIndirect)
                    {
                        return CO_MAX.powerIndirectOffBonus;
                    }
                    return CO_MAX.powerOtherBonus;
                }
                case GameEnums.PowerMode_Power:
                {
                    if (isDirect)
                    {
                        return CO.powerOffBonus;
                    }
                    else if (isIndirect)
                    {
                        return CO_MAX.powerIndirectOffBonus;
                    }
                    return CO_MAX.powerOtherBonus;
                }
                default:
                {
                    var inCoZone = co.inCORange(Qt.point(atkPosX, atkPosY), attacker);
                    if (inCoZone)
                    {
                        if (isDirect)
                        {
                            return CO_MAX.d2dCoZoneOffBonus;
                        }
                        else if (isIndirect)
                        {
                            return CO_MAX.d2dCoZoneIndirectOffBonus;
                        }
                        else
                        {
                            return CO_MAX.d2dCoZoneOtherOffBonus;
                        }
                    }
                    else
                    {
                        if (isDirect)
                        {
                            return CO_MAX.d2dOffBonus;
                        }
                        else if (isIndirect)
                        {
                            return CO_MAX.d2dIndirectOffBonus;
                        }
                        else
                        {
                            return CO_MAX.d2dOtherOffBonus;
                        }
                    }
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
                return CO_MAX.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_MAX.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (unit.getBaseMaxRange() > 1)
            {
                return CO_MAX.d2dIndirectFirerangeMalus;
            }
        }
        return 0;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {
                if (unit.getBaseMaxRange() === 1 &&
                        unit.getUnitType() !== GameEnums.UnitType_Infantry)
                {
                    return CO_MAX.powerMovementBonus;
                }
            }
            else if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                     co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                if (unit.getBaseMaxRange() === 1 &&
                        unit.getUnitType() !== GameEnums.UnitType_Infantry)
                {
                    return CO_MAX.superpowerMovementBonus;
                }
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getBaseMaxRange() === 1 &&
            unit.getUnitType() !== GameEnums.UnitType_Infantry)
        {
            return 3;
        }
        else if (unit.getBaseMaxRange() > 1)
        {
            return -3;
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getBaseMaxRange() === 1 &&
            unit.getUnitType() !== GameEnums.UnitType_Infantry)
        {
            return 3;
        }
        else if (unit.getBaseMaxRange() > 1)
        {
            return -3;
        }
        return 0;
    };

    this.getAiCoBuildRatioModifier = function(co, map)
    {
        return 10;
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
                return ["ZCOUNIT_TANK_HUNTER"];
            }
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A brave and loyal friend, not to mention a strong fighter. Max hates any kind of treachery, preferring a good, old-fashioned brawl.");
    };
    this.getHits = function(co)
    {
        return qsTr("Weight Training");
    };
    this.getMiss = function(co)
    {
        return qsTr("Studying");
    };
    this.getCODescription = function(co)
    {
        return qsTr("<r>Strong non-infantry direct-combat units, weaker indirect-combat units.</r>");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("<r>\n\nDay-to-day: \nMax's non-infantry direct-combat units gain </r><div c='#55ff00'>+%0%</div><r> firepower and his indirect-combat units have </r><div c='#ff2626'>-%1%</div><r> firepower and </r><div c='#ff2626'>-%2 range</div><r> penalty.</r>") +
        ("<r>\n\nSpecial Unit:\nTank Hunter</r>") +
        qsTr("<r>\n\nCO Zone Effect: \nMax's non-infantry direct-combat units firepower raises by </r><div c='#55ff00'>+%3%</div><r> and all others gain </r><div c='#55ff00'>+%4%</div><r> firepower and all units defense raises by </r><div c='#55ff00'>+%5%</div><r>.</r>");
        text = replaceTextArgs(text, [CO_MAX.d2dOffBonus, CO_MAX.d2dIndirectOffBonus, CO_MAX.d2dIndirectFirerangeMalus, CO_MAX.d2dCoZoneOffBonus, CO_MAX.d2dCoZoneOtherOffBonus, CO_MAX.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("<r>Max's non-infantry direct-combat units gain </r><div c='#55ff00'>+%0 movement</div><r> and raise their firepower by </r><div c='#55ff00'>+%1%</div><r> and all other units firepower raises by </r><div c='#55ff00'>+%2%</div><r>. All units defense raises by </r><div c='#55ff00'>+%3%</div><r>.</r>");
        text = replaceTextArgs(text, [CO_MAX.powerMovementBonus, CO_MAX.powerOffBonus, CO_MAX.powerOtherBonus, CO_MAX.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Max Force");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("<r>Max's non-infantry direct-combat units gain </r><div c='#55ff00'>+%0 movement</div><r> and raise their firepower by </r><div c='#55ff00'>+%1%</div><r> and all other units firepower raises by </r><div c='#55ff00'>+%2%</div><r>. All units defense raises by </r><div c='#55ff00'>+%3%</div><r>.</r>");
        text = replaceTextArgs(text, [CO_MAX.superpowerMovementBonus, CO_MAX.superPowerOffBonus, CO_MAX.powerOtherBonus, CO_MAX.powerDefBonus]);
        return text;
    };

    this.getSuperPowerName = function(co)
    {
        return qsTr("Max Blast");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Roll, tanks, roll!"),
            qsTr("Now you're gonna get hurt!"),
            qsTr("Hey!  Give up while you still can!"),
            qsTr("Wanna test might?  I won't lose!"),
            qsTr("That's enough!  Get outta the road!"),
            qsTr("Alright, the gloves are comin' off.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("That was a piece of cake!"),
            qsTr("Ha! It'll take more than that to beat me!"),
            qsTr("I'm on a roll!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Ouch... I let my guard down."),
            qsTr("Oh, man! Not good! What are we supposed to do now!?")];
    };
    this.getName = function()
    {
        return qsTr("Max");
    };
}

Constructor.prototype = CO;
var CO_MAX = new Constructor();
