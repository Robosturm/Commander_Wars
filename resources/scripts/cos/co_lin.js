var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(2);
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
                audio.addMusic("resources/music/cos/lin.mp3", 3991, 61044);
                break;
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 1;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        var seaAirUnit = (attacker.getUnitType() === GameEnums.UnitType_Air) ||
                         (attacker.getUnitType() === GameEnums.UnitType_Naval);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (!seaAirUnit)
                {
                    return 50;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                if (!seaAirUnit)
                {
                    return 30;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (!seaAirUnit)
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
        var seaAirUnit = (defender.getUnitType() === GameEnums.UnitType_Air) ||
                         (defender.getUnitType() === GameEnums.UnitType_Naval);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (!seaAirUnit)
                {
                    return 50;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                if (!seaAirUnit)
                {
                    return 30;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), defender))
                {
                    if (!seaAirUnit)
                    {
                        return 30;
                    }
                    return 10;
                }
                break;
        }
        return 0;
    };

    this.getVisionrangeModifier = function(co, unit, posX, posY, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 2;
            case GameEnums.PowerMode_Power:
                return 1;
            default:
                return 0;
        }
    };

    this.getPerfectVision = function(co, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return true;
            default:
                return false;
        }
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        var seaAirUnit = (unit.getUnitType() === GameEnums.UnitType_Air) ||
                         (unit.getUnitType() === GameEnums.UnitType_Naval);

        if (!seaAirUnit)
        {
            return 2;
        }
        return 1;
    };
    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_INTEL_TRUCK"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("In battle the clearest thinker wins. A calm commander of Amber Corona.");
    };
    this.getHits = function(co)
    {
        return qsTr("Teasing Will");
    };
    this.getMiss = function(co)
    {
        return qsTr("Flying");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Her ground units are very strong. Other units have normal power.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nIntel truck\n") +
               qsTr("\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nGround Units gain 30% firepower and defense.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Increases vision range by 1 and allows her to view into hidden places. Also increases the power of her ground units.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Enhanced Vision");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Increases vision range by 2 and allows her to view into hidden places. Also increases the power of her ground units.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Night Vision");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Begin search and destroy operations. Bring the fight to them."),
                qsTr("It's not about firepower. The force with the most information wins."),
                qsTr("I see enemies in the vicinity."),
                qsTr("All units move out. Discipline must be maintain")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Now we have some breathing space..."),
                qsTr("I predicted this outcome...")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("We had not enough intel."),
                qsTr("Next time I am better prepared.")];
    };
    this.getName = function()
    {
        return qsTr("Lin");
    };
}

Constructor.prototype = CO;
var CO_LIN = new Constructor();
