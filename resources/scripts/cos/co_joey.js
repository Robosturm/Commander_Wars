var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
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
                audio.addMusic("resources/music/cos/bh_power.mp3", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/joey.mp3")
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
        return "TI";
    };

    this.superPowerOffMalus = 0;
    this.superPowerOffBonus = 60;
    this.superPowerBaseOffBonus = 10;
    this.superPowerDefBonus = 10;
    this.superPowerCostReduction = 0.2;

    this.powerOffMalus = 0;
    this.powerOffBonus = 60;
    this.powerBaseOffBonus = 10;
    this.powerDefBonus = 30;
    this.powerBaseDefBonus = 10;

    this.d2dOffMalus = -10;
    this.d2dOffBonus = 20;
    this.d2dBaseOffBonus = 0;

    this.d2dCoZoneOffMalus = 0;
    this.d2dCoZoneOffBonus = 60;
    this.d2dCoZoneBaseOffBonus = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            let attackerValue = 0;
            let defenderValue = 0;
            if(defender !== null)
            {
                attackerValue = attacker.getUnitValue();
                defenderValue = defender.getUnitValue();
            }
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            {
                if (attackerValue > defenderValue)
                {
                    return CO_JOEY.superPowerOffMalus;
                }
                else if (attackerValue < defenderValue)
                {
                    return CO_JOEY.superPowerOffBonus;
                }
                else
                {
                    return CO_JOEY.superPowerBaseOffBonus;
                }
            }
            case GameEnums.PowerMode_Power:
            {
                if (attackerValue > defenderValue)
                {
                    return CO_JOEY.powerOffMalus;
                }
                else if (attackerValue < defenderValue)
                {
                    return CO_JOEY.powerOffBonus;
                }
                else
                {
                    return CO_JOEY.powerBaseOffBonus;
                }
            }
            default:
            {
                if (attackerValue > defenderValue)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return CO_JOEY.d2dCoZoneOffMalus;
                    }
                    return CO_JOEY.d2dOffMalus;
                }
                else if (attackerValue < defenderValue)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return CO_JOEY.d2dCoZoneOffBonus;
                    }
                    return CO_JOEY.d2dOffBonus;
                }
                else
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return CO_JOEY.d2dCoZoneBaseOffBonus;
                    }
                    return CO_JOEY.d2dBaseOffBonus;
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
            let attackerValue = 0;
            let defenderValue = 0;
            if(attacker !== null)
            {
                attackerValue = attacker.getUnitValue();
                defenderValue = defender.getUnitValue();
            }
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_JOEY.superPowerDefBonus;
            case GameEnums.PowerMode_Power:
                if (attackerValue > defenderValue)
                {
                    return CO_JOEY.powerDefBonus;
                }
                return CO_JOEY.powerBaseDefBonus;
            default:
            }
        }
        return 0;
    };

    this.getFirstStrike = function(co, unit, posX, posY, attacker, isDefender, map, atkPosX, atkPosY)
    {
        if (CO.isActive(co))
        {
            if(unit !== null && isDefender)
            {
                var defenderValue = unit.getUnitValue();
                var attackerValue = attacker.getUnitValue();
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    if (attackerValue > defenderValue)
                    {
                        return true;
                    }
                    return false;
                case GameEnums.PowerMode_Power:
                    return false;
                default:
                    return false;
                }
            }
        }
        return false;
    };

    this.getCostModifier = function(co, id, baseCost, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return -baseCost * CO_JOEY.superPowerCostReduction;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                return 0;
            }
        }
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Loves to live and fight on the edge. He prefers risks and gambles over safety.");
    };
    this.getHits = function(co)
    {
        return qsTr("Disadvantages");
    };
    this.getMiss = function(co)
    {
        return qsTr("Cheaters");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Joey likes to live on the edge. Units are stronger when engaging stronger units, but firepower is reduced when engaging a weaker unit.");
    };
    this.getLongCODescription = function()
    {
        let text = qsTr("\nGlobal Effect: \nUnits gain %0% firepower when engaging stronger units, but firepower is reduced by %1% when fighting a weaker unit.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain %0% firepower when engaging stronger units, but firepower is reduced by %1% when fighting a weaker unit.");
        text = replaceTextArgs(text, [CO_JOEY.d2dOffBonus, CO_JOEY.d2dOffMalus,
                                      CO_JOEY.d2dCoZoneOffBonus, CO_JOEY.d2dCoZoneOffMalus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        let text = qsTr("Units receive %0% firepower and %1% defence boost when fighting a stronger unit.");
        text = replaceTextArgs(text, [CO_JOEY.powerOffBonus, CO_JOEY.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Eccentricity");
    };
    this.getSuperPowerDescription = function(co)
    {
        let text = qsTr("Deployment costs drops by %0% and units get first strike when fighting a stronger unit. Units gain %0% firepower when engaging stronger units, but firepower is reduced by %1% when fighting a weaker unit.");
        text = replaceTextArgs(text, [CO_JOEY.superPowerCostReduction * 100, CO_JOEY.powerDefBonus, CO_JOEY.superPowerOffBonus, CO_JOEY.superPowerOffMalus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Tempestuous Technique");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I feel a bit dirty for doing this..."),
                qsTr("I gave you a chance, but you didn't want it."),
                qsTr("You gotta live life on the edge!"),
                qsTr("You were never too good."),
                qsTr("You're going to be feeling edgy about this..."),
                qsTr("C'mon, give the little guys a chance!"),
                qsTr("What's a little risk taking going to hurt?")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Heh, good game."),
                qsTr("Power isn't everything y'know..."),
                qsTr("Big risk, big reward!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("This was to risky..."),
                qsTr("I thought my risk was calculated but man i'm bad at math!")];
    };
    this.getName = function()
    {
        return qsTr("Joey");
    };
}

Constructor.prototype = CO;
var CO_JOEY = new Constructor();
