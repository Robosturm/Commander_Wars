var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co, map)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power_awdc.mp3", 0, 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_awdc.mp3", 0, 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/gage.mp3",9315 ,70409)
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
            if (unit.getBaseMaxRange() > 1 || unit.getUnitType() === GameEnums.UnitType_Naval)
            {
                var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
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
            if (unit.getBaseMaxRange() > 1 || unit.getUnitType() === GameEnums.UnitType_Naval)
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
        return 2;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getBaseMaxRange() > 1 ||
                    attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return 50;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                if (attacker.getBaseMaxRange() > 1 ||
                    attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return 30;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getBaseMaxRange() > 1 ||
                        attacker.getUnitType() === GameEnums.UnitType_Naval)
                    {
                        return 30;
                    }
                    return 10;
                }
                break;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (defender.getBaseMaxRange() > 1 ||
                    defender.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return 30;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                if (defender.getBaseMaxRange() > 1 ||
                    defender.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return 20;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    if (defender.getBaseMaxRange() > 1 ||
                        defender.getUnitType() === GameEnums.UnitType_Naval)
                    {
                        return 20;
                    }
                    return 10;
                }
                break;
        }
        return 0;
    };

    this.getFirerangeModifier = function(co, unit, posX, posY, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (unit.getBaseMaxRange() > 1)
                {
                    return 3;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (unit.getBaseMaxRange() > 1)
                {
                    return 2;
                }
                break;
            default:
                break;
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getBaseMaxRange() > 1 ||
            unit.getUnitType() === GameEnums.UnitType_Naval)
        {
            return 6;
        }
        return 0;
    };

    this.getAiCoBuildRatioModifier = function(co, map)
    {
        return 0.4;
    };

    this.getCOArmy = function()
    {
        return "BD";
    };
    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_SIEGE_CANNON"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Brown Desert Army soldier. A man of few words but a consummate professional");
    };
    this.getHits = function(co)
    {
        return qsTr("Getting things done");
    };
    this.getMiss = function(co)
    {
        return qsTr("Talking");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Indirect-combat units and sea units cause more damage and have increased firerange.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nSiege Cannon\n") +
               qsTr("\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nIndirect and Sea Units have 30% offensive and 20% defensive bonus.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Increases range of indirect units by two space. Firepower of sea and indirect units also rise.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Longshot");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Increases range of indirect units by three spaces. Firepower of sea and indirect units greatly rise.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Snipershot");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Our business is war. We should no emotions let us distract from our task."),
                qsTr("At this distance it should be with'n firerange..."),
                qsTr("Prepare to open fire!"),
                qsTr("It is not my job to think about that.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Now let us prepare for the next battle."),
                qsTr("I have carried out my duties"),
                qsTr("It is not my job to think about that.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Missed shot!"),
                qsTr("What i didn't hit you?")];
    };
    this.getName = function()
    {
        return qsTr("Gage");
    };
}

Constructor.prototype = CO;
var CO_GAGE = new Constructor();
