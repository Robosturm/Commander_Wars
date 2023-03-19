var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
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
        var size = units.size();
        for (var i = 0; i < size; i++)
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
                animation.setSound("power7_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power7_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
        var size = units.size();
        for (var i = 0; i < size; i++)
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
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/xavier.mp3", 270, 74593);
                break;
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "GS";
    };
    this.superpowerBonus = 60;
    this.firepowerBonus = 20;
    this.luckDamage = 40;
    this.minLuckHp = 5;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                var count = 0;
                if (CO_XAVIER.isAlliedUnit(attacker, defPosX, defPosY + 1, map))
                {
                    count++;
                }
                if (CO_XAVIER.isAlliedUnit(attacker, defPosX, defPosY - 1, map))
                {
                    count++;
                }
                if (CO_XAVIER.isAlliedUnit(attacker, defPosX + 1, defPosY, map))
                {
                    count++;
                }
                if (CO_XAVIER.isAlliedUnit(attacker, defPosX - 1, defPosY, map))
                {
                    count++;
                }
                return CO_XAVIER.firepowerBonus + CO_XAVIER.superpowerBonus * count;
            case GameEnums.PowerMode_Power:
                return CO_XAVIER.firepowerBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_XAVIER.firepowerBonus;
                }
                break;
        }
        return 0;
    };
    this.isAlliedUnit = function(attacker, x, y, map)
    {
        if (map.onMap(x, y))
        {
            var unit = map.getTerrain(x, y).getUnit();
            if (unit !== null &&
                attacker.getOwner() === unit.getOwner() &&
                attacker !== unit)
            {
                return true;
            }
        }
        return false;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) || co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    this.getBonusLuck = function(co, unit, posX, posY, map)
    {
         var hpRounded = unit.getHpRounded();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (hpRounded <= CO_XAVIER.minLuckHp)
                {
                    return hpRounded / 2;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (hpRounded <= CO_XAVIER.minLuckHp)
                {
                    return CO_XAVIER.luckDamage + hpRounded / 2;
                }
                return CO_XAVIER.luckDamage;
            default:
                if (hpRounded <= CO_XAVIER.minLuckHp)
                {
                    return hpRounded / 2;
                }
        }
        return 0;
    };

    this.getBonusMisfortune = function(co, unit, posX, posY, map)
    {
        var hpRounded = unit.getHpRounded();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (hpRounded <= CO_XAVIER.minLuckHp)
                {
                    return -hpRounded;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (hpRounded <= CO_XAVIER.minLuckHp)
                {
                    return -CO_XAVIER.luckDamage + -hpRounded;
                }
                return -CO_XAVIER.luckDamage;
            default:
                if (hpRounded <= CO_XAVIER.minLuckHp)
                {
                    return -hpRounded;
                }
                break;
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
        return qsTr("Fulfills his duties without second thought or consideration of the after-effects of his actions. Wears a pair of fake claws.");
    };
    this.getHits = function(co)
    {
        return qsTr("Uncertainty");
    };
    this.getMiss = function(co)
    {
        return qsTr("Definitives");
    };
    this.getCODescription = function(co)
    {
        var text = qsTr("When Xavier's units drop to %0 HP or less, they are able to strike for maximum luck damage.");
        text = replaceTextArgs(text, [CO_XAVIER.minLuckHp]);
        return text;
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nGlobal Effect: \nUnits with %0 HP or less deal maximum luck damage.") +
                   qsTr("\n\nCO Zone Effect: \nUnits have increased firepower by %1%.");
        text = replaceTextArgs(text, [CO_XAVIER.minLuckHp, CO_XAVIER.firepowerBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text =  qsTr("Increases luck damage to %0%.");
        text = replaceTextArgs(text, [CO_XAVIER.luckDamage]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Phasing Charge");
    };
    this.getSuperPowerDescription = function(co)
    {
         var text =  qsTr("Units gain %0% firepower for each own unit nearby the attacked unit.");
        text = replaceTextArgs(text, [CO_XAVIER.superpowerBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Reality Minus");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("You know, the laws of physics are made to be broken. Observe."),
                qsTr("You might say I'm a rather.. twisted individual."),
                qsTr("Reality is only trivial. Watch and learn."),
                qsTr("Allow me to reprimand, for your atrocious command."),
                qsTr("I'm sure you'll get quite a scare out of this!"),
                qsTr("Are you sure of the truth in what you see?")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("... I must return to my work now."),
                qsTr("See, this is reality. You never stood a chance."),
                qsTr("You are intellectually inferior to me. Simple as that.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("That's not my reality."),
                qsTr("You were supposed to be intellectually inferior to me.")];
    };
    this.getName = function()
    {
        return qsTr("Xavier");
    };
}

Constructor.prototype = CO;
var CO_XAVIER = new Constructor();
