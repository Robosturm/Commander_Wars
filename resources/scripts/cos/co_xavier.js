var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
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
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.ogg", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.ogg", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/xavier.ogg", 270, 74593);
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
        return "GS";
    };
    this.superpowerBonus = 60;

    this.powerFirepowerBonus = 20;
    this.powerLuckDamage = 40;
    this.powerDefBonus = 10;

    this.d2dCoZoneFirepowerBonus = 20;
    this.d2dCoZoneDefBonus = 10;

    this.d2dMinLuckHp = 5;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
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
                return CO_XAVIER.powerFirepowerBonus + CO_XAVIER.superpowerBonus * count;
            case GameEnums.PowerMode_Power:
                return CO_XAVIER.powerFirepowerBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_XAVIER.d2dCoZoneFirepowerBonus;
                }
                break;
            }
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
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_XAVIER.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_XAVIER.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getBonusLuck = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            var hpRounded = unit.getHpRounded();
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (hpRounded <= CO_XAVIER.d2dMinLuckHp)
                {
                    return hpRounded / 2;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (hpRounded <= CO_XAVIER.d2dMinLuckHp)
                {
                    return CO_XAVIER.powerLuckDamage + hpRounded / 2;
                }
                return CO_XAVIER.powerLuckDamage;
            default:
                if (hpRounded <= CO_XAVIER.d2dMinLuckHp)
                {
                    if (map === null ||
                        (map !== null && map.getGameRules().getCoGlobalD2D()) ||
                         co.inCORange(Qt.point(posX, posY), unit))
                    {
                        return hpRounded / 2;
                    }
                }
            }
        }
        return 0;
    };

    this.getBonusMisfortune = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            var hpRounded = unit.getHpRounded();
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (hpRounded <= CO_XAVIER.d2dMinLuckHp)
                {
                    return -hpRounded;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (hpRounded <= CO_XAVIER.d2dMinLuckHp)
                {
                    return -CO_XAVIER.powerLuckDamage + -hpRounded;
                }
                break;
            default:
                if (hpRounded <= CO_XAVIER.d2dMinLuckHp)
                {
                    if (map === null ||
                        (map !== null && map.getGameRules().getCoGlobalD2D()) ||
                         co.inCORange(Qt.point(posX, posY), unit))
                    {
                        return -hpRounded;
                    }
                }
                break;
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
        text = replaceTextArgs(text, [CO_XAVIER.d2dMinLuckHp]);
        return text;
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_XAVIER.d2dMinLuckHp];
        }
        var text = qsTr("\nGlobal Effect: \nXavier's units with %0 HP or less deal maximum luck damage.") +
                   qsTr("\n\nCO Zone Effect: \nXavier's units with %1 HP or less deal maximum luck damage. Xavier's units gain +%2% firepower and +%3% defence.");
        text = replaceTextArgs(text, [values[0], CO_XAVIER.d2dMinLuckHp, CO_XAVIER.d2dCoZoneFirepowerBonus, CO_XAVIER.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text =  qsTr("Xavier's units gain +%0 luck, +%1% firepower, and +%2% defence.");
        text = replaceTextArgs(text, [CO_XAVIER.powerLuckDamage, CO_XAVIER.powerFirepowerBonus, CO_XAVIER.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Phasing Charge");
    };
    this.getSuperPowerDescription = function(co)
    {
         var text =  qsTr("Xavier's units gain +%1% firepower and +%2% defence. When his units attack, they gain an additional +%0% firepower for each of his other units adjacent to the attacked unit.");
        text = replaceTextArgs(text, [CO_XAVIER.superpowerBonus, CO_XAVIER.powerFirepowerBonus, CO_XAVIER.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Reality Minus");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("You know, the laws of physics are made to be broken. Observe."),
                qsTr("You might say I'm a rather...twisted individual."),
                qsTr("Reality is only trivial. Watch and learn."),
                qsTr("Allow me to reprimand, for your atrocious command."),
                qsTr("I'm sure you'll get quite a scare out of this!"),
                qsTr("Are you sure of the truth in what you see?")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I must return to my own work now."),
                qsTr("See, this is reality. You never stood a chance."),
                qsTr("You are intellectually inferior to me. Simple as that.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("This is not my reality."),
                qsTr("You were supposed to be intellectually inferior to me.")];
    };
    this.getName = function()
    {
        return qsTr("Xavier");
    };
}

Constructor.prototype = CO;
var CO_XAVIER = new Constructor();
