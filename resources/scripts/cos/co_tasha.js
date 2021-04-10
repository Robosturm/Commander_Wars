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
            if (unit.getUnitType() === GameEnums.UnitType_Air)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                if (globals.randInt(0, 1) === 0)
                {
                    animation.setSound("power2_1.wav");
                }
                else
                {
                    animation.setSound("power2_2.wav");
                }
                if (animations.length < 5)
                {
                    animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
            if (unit.getUnitType() === GameEnums.UnitType_Air)
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
                audio.addMusic("resources/music/cos/power_awdc.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_awdc.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/tasha.mp3", 1663, 66947);
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 1;
    };
    this.getCOArmy = function()
    {
        return "BD";
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return 70;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return 50;
                }
                else
                {
                    return 10;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getUnitType() === GameEnums.UnitType_Air)
                    {
                        return 50;
                    }
                    return 10;
                }
                break;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return 30;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return 30;
                }
                else
                {
                    return 10;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), defender))
                {
                    if (defender.getUnitType() === GameEnums.UnitType_Air)
                    {
                        return 30;
                    }
                    return 10;
                }
                break;
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return 2;
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 1;
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        if (unit.getUnitType() === GameEnums.UnitType_Air)
        {
            return 2;
        }
        return 0;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Brown Desert soldier. Burns with desire to avenge her late brother. Aims to make Teal Isles pay for what they did.");
    };
    this.getHits = function(co)
    {
        return qsTr("Her brother John, Revenge");
    };
    this.getMiss = function(co)
    {
        return qsTr("Teal Isles");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Air units have higher firepower and defense.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nNo Effects") +
               qsTr("\n\nCO Zone Effect: \nAir Units have 50% increased firepower and 30% increased defense.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Air units have higher firepower, defense and all units can move 1 space farther.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Sonic Boom");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Air units have impressive firepower, higher defense and all units can move 2 space farther");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Fox One");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("All units follow my lead."),
                qsTr("The time for revenge is at hand!"),
                qsTr("I will fight on, even if it costs my life!"),
                qsTr("I will have my revenge!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("It's not about tactics - it's about passion!"),
                qsTr("Revenge will be mine!"),
                qsTr("I will have my revenge!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("This is not the end. I will fight for John."),
                qsTr("Next time... Next time i will avenge you John...")];
    };
    this.getName = function()
    {
        return qsTr("Tasha");
    };
}

Constructor.prototype = CO;
var CO_TASHA = new Constructor();
