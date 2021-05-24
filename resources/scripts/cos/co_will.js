var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.activatePower = function(co)
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
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                if (globals.randInt(0, 1) === 0)
                {
                    animation.setSound("power10_1.wav");
                }
                else
                {
                    animation.setSound("power10_2.wav");
                }
                if (animations.length < 5)
                {
                    animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        units.remove();
    };

    this.activateSuperpower = function(co, powerMode)
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
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                if (globals.randInt(0, 1) === 0)
                {
                    animation.setSound("power12_1.wav");
                }
                else
                {
                    animation.setSound("power12_2.wav");
                }
                if (animations.length < 5)
                {
                    animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
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
        units.remove();
    };

    this.loadCOMusic = function(co)
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
                audio.addMusic("resources/music/cos/will.mp3", 36421, 91424);
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
        var seaAirUnit = (attacker.getUnitType() === GameEnums.UnitType_Air) ||
                         (attacker.getUnitType() === GameEnums.UnitType_Naval);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
                {
                    return 50;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
                {
                    return 30;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
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
                                       defender, defPosX, defPosY, isDefender, action)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        var seaAirUnit = (unit.getUnitType() === GameEnums.UnitType_Air) ||
                         (unit.getUnitType() === GameEnums.UnitType_Naval);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (!seaAirUnit)
                {
                    return 2;
                }
                return 0;
            case GameEnums.PowerMode_Power:
                if (!seaAirUnit)
                {
                    return 1;
                }
                return 0;
            default:
                return 0;
        }
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        var seaAirUnit = (unit.getUnitType() === GameEnums.UnitType_Air) ||
                         (unit.getUnitType() === GameEnums.UnitType_Naval);
        if (unit.getBaseMaxRange() === 1 && !seaAirUnit)
        {
            return 2;
        }
        return 0;
    };
    this.getCOUnits = function(co, building)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_HOT_TANK"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Former Brown Desert cadet. Joined Amber Corona after being rescued by Brenner.");
    };
    this.getHits = function(co)
    {
        return qsTr("Brenner, Isabella");
    };
    this.getMiss = function(co)
    {
        return qsTr("The Beast, Greyfield, Caulder");
    };
    this.getCODescription = function(co)
    {
        return qsTr("His direct ground units are slightly stronger.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nHot Tank\n") +
               qsTr("\nGlobal Effect:\nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nDirect Ground Units have 30% increased firepower.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Boost movement by 1 and increases the firepower of direct ground units.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Begin of an Era");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Boost movement by 2 and firepower of direct ground units.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("New Era");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("This is our chance to win. Prepare to move out."),
                qsTr("The captain saved my life. I need to take care of it."),
                qsTr("I'll never give up..."),
                qsTr("There's no time. Now or never...")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I did it!"),
                qsTr("I'll never give up..."),
                qsTr("Is everyone okay?")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Never give up hope."),
                qsTr("I can't remember anything")];
    };
    this.getName = function()
    {
        return qsTr("Will");
    };
}

Constructor.prototype = CO;
var CO_WILL = new Constructor();
