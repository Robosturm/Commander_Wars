var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
        return ["+alt", "+alt2", "+alt3"];
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAtEnd(co, powerSurplus, turnMode);
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(6);
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
            if (unit.getUnitType() !== GameEnums.UnitType_Infantry)
            {
                unit.setHasMoved(false);
                var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                var delay = globals.randInt(135, 265);
                if (animations.length < 5)
                {
                    delay *= i;
                }
                animation.setSound("power1.wav", 1, delay);
                if (animations.length < 5)
                {
                    animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, delay);
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, delay);
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
            if (unit.getUnitType() !== GameEnums.UnitType_Infantry)
            {
                unit.setHasMoved(false);
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

    this.loadCOMusic = function(co, map)
    {
        // put the co music in here.
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
                audio.addMusic("resources/music/cos/eagle.mp3", 27, 67923)
                break;
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "GE";
    };

    this.getFuelCostModifier = function(co, unit, costs, map)
    {
        if (unit.getUnitType() === GameEnums.UnitType_Air)
        {
            return -2;
        }
        return 0;
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return 30;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return -30;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return -40;
                }
                else
                {
                    return -45;
                }
            default:
                if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return 30;
                    }
                    return 10;
                }
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                    {
                        return 0;
                    }
                    return 10;
                }
                break;
        }
        if (attacker.getUnitType() === GameEnums.UnitType_Naval)
        {
            return -10;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getUnitType() === GameEnums.UnitType_Naval)
        {
            return -2;
        }
        else if (unit.getUnitType() === GameEnums.UnitType_Air)
        {
            return 6;
        }
        return 0;
    };
    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "AIRPORT" ||
            buildingId === "TEMPORARY_AIRPORT")
        {
            return ["ZCOUNIT_KIROV"];
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Green Earth's daring pilot hero. Joined the air force to honor his father's legacy.");
    };
    this.getHits = function(co)
    {
        return qsTr("Lucky goggles");
    };
    this.getMiss = function(co)
    {
        return qsTr("Swimming");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Air units use less fuel and have superior firepower. Naval units have weaker firepower.");
    };
    this.getLongCODescription = function()
    {
        return  qsTr("\nSpecial Unit:\nKirov\n") +
                qsTr("\nGlobal Effect: \nAir units are stronger and use less fuel, but Sea units are weaker in comparison.") +
                qsTr("\n\nCO Zone Effect: \nAir units have increased firepower.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("All non-infantry units that have already carried out orders may move again, but their firepower is cut in half.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Lightning Drive");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("All non-infantry units that have already carried out orders may move again with no firepower penalty.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Lightning Strike");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Checkmate!"),
                qsTr("All units, ready for attack! Don't even give them time for regret!"),
                qsTr("Do you think you can keep up?"),
                qsTr("The Eagle soars above you!"),
                qsTr("I have no interest in underlings. Begone!"),
                qsTr("I've caught you with your guard down!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Hmph! As expected!"),
                qsTr("Where's the challenge in that?"),
                qsTr("Another victory! Was there ever any doubt?")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("This is what happens when you lose focus!"),
                qsTr("Impossible! I'm getting angrier by the moment!")];
    };
    this.getName = function()
    {
        return qsTr("Eagle");
    };
}

Constructor.prototype = CO;
var CO_EAGLE = new Constructor();
