var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
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
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();

        audio.clearPlayList();
        CO_XAVIER.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
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
        units.remove();

        audio.clearPlayList();
        CO_XAVIER.loadCOMusic(co);
        audio.playRandom();
    };

    this.loadCOMusic = function(co)
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

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "GS";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                var count = 0;
                if (CO_XAVIER.isAlliedUnit(attacker, defPosX, defPosY + 1))
                {
                    count++;
                }
                if (CO_XAVIER.isAlliedUnit(attacker, defPosX, defPosY - 1))
                {
                    count++;
                }
                if (CO_XAVIER.isAlliedUnit(attacker, defPosX + 1, defPosY))
                {
                    count++;
                }
                if (CO_XAVIER.isAlliedUnit(attacker, defPosX - 1, defPosY))
                {
                    count++;
                }
                return 10 + 60 * count;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 20;
                }
                break;
        }
        return 0;
    };
    this.isAlliedUnit = function(attacker, x, y)
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
                                       defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };

    this.getBonusLuck = function(co, unit, posX, posY)
    {
         var hpRounded = unit.getHpRounded();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (hpRounded <= 5)
                {
                    return hpRounded / 2;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (hpRounded <= 5)
                {
                    return 20 + hpRounded / 2;
                }
                return 20;
            default:
                if (hpRounded <= 5)
                {
                    return hpRounded / 2;
                }
        }
        return 0;
    };

    this.getBonusMisfortune = function(co, unit, posX, posY)
    {
        var hpRounded = unit.getHpRounded();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (hpRounded <= 5)
                {
                    return -hpRounded;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (hpRounded <= 5)
                {
                    return -20 + -hpRounded;
                }
                return -20;
            default:
                if (hpRounded <= 5)
                {
                    return -hpRounded;
                }
                break;
        }
        return 0;
    };
    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Fulfills his duties without second thought or consideration of the after-effects of his actions. Wears a pair of fake claws.");
    };
    this.getHits = function()
    {
        return qsTr("Uncertainty");
    };
    this.getMiss = function()
    {
        return qsTr("Definitives");
    };
    this.getCODescription = function()
    {
        return qsTr("When Xavier's units drop below 5 HP, they are able to strike for maximum luck damage.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Attacks inflict more damage than expected.");
    };
    this.getPowerName = function()
    {
        return qsTr("Phasing Charge");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Units gain firepower for each own unit nearby the attacked unit.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Reality Minus");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("You know, the laws of physics are made to be broken. Observe."),
                qsTr("You might say I'm a rather.. twisted individual."),
                qsTr("Reality is only trivial. Watch and learn."),
                qsTr("Allow me to reprimand, for your atrocious command."),
                qsTr("I'm sure you'll get quite a scare out of this!"),
                qsTr("Are you sure of the truth in what you see?")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("... I must return to my work now."),
                qsTr("See, this is reality. You never stood a chance."),
                qsTr("You are intellectually inferior to me. Simple as that.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("That's not my realitiy."),
                qsTr("You were supposed to be intellectually inferior to me.")];
    };
    this.getName = function()
    {
        return qsTr("Xavier");
    };
}

Constructor.prototype = CO;
var CO_XAVIER = new Constructor();
