var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
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
            if (i % 2 === 0)
            {
                animation.setSound("power10_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power10_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power10", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power10", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
        if (CO.isActive(co))
        {
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
                audio.addMusic("resources/music/cos/isabella.mp3")
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };

    this.superPowerOffBonus = 30;
    this.superPowerDefBonus = 30;
    this.superPowerFirerangeBonus = 2;
    this.superPowerMovementBonus = 2;

    this.powerOffBonus = 20;
    this.powerDefBonus = 20;
    this.powerFirerangeBonus = 1;
    this.powerMovementBonus = 1;

    this.d2dOffBonus = 0;
    this.d2dDefBonus = 0;

    this.d2dCoZoneOffBonus = 20;
    this.d2dCoZoneDefBonus = 20;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_ISABELLA.superPowerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_ISABELLA.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_ISABELLA.d2dCoZoneOffBonus;
                }
                return CO_ISABELLA.d2dOffBonus;
            }
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_ISABELLA.superPowerDefBonus;
            case GameEnums.PowerMode_Power:
                return CO_ISABELLA.powerDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_ISABELLA.d2dCoZoneDefBonus;
                }
                return CO_ISABELLA.d2dDefBonus;
            }
        }
        return 0;
    };

    this.getFirerangeModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (unit.getBaseMaxRange() > 1)
                {
                    return CO_ISABELLA.superPowerFirerangeBonus;
                }
                return 0;
            case GameEnums.PowerMode_Power:
                if (unit.getBaseMaxRange() > 1)
                {
                    return CO_ISABELLA.powerFirerangeBonus;
                }
                return 0;
            default:
                return 0;
            }
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_ISABELLA.superPowerMovementBonus;
            case GameEnums.PowerMode_Power:
                return CO_ISABELLA.powerMovementBonus;
            default:
                return 0;
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Has no memory of her past. Rescued by Will, she has joined Amber Corona.");
    };
    this.getHits = function(co)
    {
        return qsTr("Will");
    };
    this.getMiss = function(co)
    {
        return qsTr("Amnesia");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Her units are slightly stronger.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nGlobal Effect: \nUnits have %0% firepower and %1% defense bonus.") +
               qsTr("\n\nCO Zone Effect: \nUnits have %2% firepower and %3% defense bonus.");
        text = replaceTextArgs(text, [CO_ISABELLA.d2dOffBonus, CO_ISABELLA.d2dDefBonus,
                                      CO_ISABELLA.d2dCoZoneOffBonus, CO_ISABELLA.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Units have %0% firepower and %1% defense bonus. Increases her movement range by %2 and indirect firerange by %3.");
        text = replaceTextArgs(text, [CO_ISABELLA.powerOffBonus, CO_ISABELLA.powerDefBonus,
                                      CO_ISABELLA.powerMovementBonus, CO_ISABELLA.powerFirerangeBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Initial Strike");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Units have %0% firepower and %1% defense bonus. Increases her movement range by %2 and indirect firerange by %3.");
        text = replaceTextArgs(text, [CO_ISABELLA.superPowerOffBonus, CO_ISABELLA.superPowerDefBonus,
                                      CO_ISABELLA.superPowerMovementBonus, CO_ISABELLA.superPowerFirerangeBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Deep Strike");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I must fight alongside the others."),
                qsTr("Launch Overlord."),
                qsTr("Doing this will lessen the casualties."),
                qsTr("I must try to optimize my fighting powers.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I am starting to remember..."),
                qsTr("The enemy have been taken care of.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Still no peace for me and Will."),
                qsTr("I can't remember anything")];
    };
    this.getName = function()
    {
        return qsTr("Isabella");
    };
}

Constructor.prototype = CO;
var CO_ISABELLA = new Constructor();
