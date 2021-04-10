var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
    };

    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
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
                audio.addMusic("resources/music/cos/javier.mp3", 22016, 74716);
                break;
        }
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
        units.remove();
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "GE";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var towers = co.getOwner().getBuildingCount("TOWER");
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return towers * 25 + 10;
            case GameEnums.PowerMode_Power:
                return towers * 15 + 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return towers * 5 + 10;
                }
        }
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var rangedAttacked = (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1);
        var towers = co.getOwner().getBuildingCount("TOWER");
        var ret = 0;
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (rangedAttacked)
                {
                    ret += 60;
                }
                ret += towers * 5  + 10;
                break;
            case GameEnums.PowerMode_Power:
                if (rangedAttacked)
                {
                    ret += 40;
                }
                ret += towers * 5  + 10;
                break;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    ret += towers * 5 + 10;
                    if (rangedAttacked)
                    {
                        ret += 20;
                    }
                }
                break;
        }
        return ret;
    };

    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A Green Earth commander who values chivalry and honor above all else. Often orders his units to charge.");
    };
    this.getHits = function(co)
    {
        return qsTr("Honor");
    };
    this.getMiss = function(co)
    {
        return qsTr("Retreating");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Units possess superior defenses vs. indirect attacks and Comtowers have a higher impact.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nUnits have increased defense against indirect units. Units gain additional firepower and defense per Comtower.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Improves defense vs. indirect attacks and firepower slightly rises the more Comtower he owns.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Tower Shield");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Units have even greater defenses vs. indirect attacks and firepower increases at a high rate the more Comtowers he owns.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Tower of Power");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Away put your weapons! You shall not be needing them where you are going!"),
                qsTr("Charge into the toothy maw! For now is the time for a glorious hindspanking!"),
                qsTr("Mighty spire of communication, imbue my blade with stabby might!"),
                qsTr("Your maidenfolk shall soon howl great lamentations!"),
                qsTr("This day shall be the greatest of days, unless tomorrow is even greater!"),
                qsTr("Foe, you are mad if you seek to pierce my steely hide!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("No sharpness can penetrate my steely hide!"),
                qsTr("Consider your hindquarters righteously spanked!"),
                qsTr("You still live for. Javier is masterful but merciful!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Well done! Your reputation is well deserved."),
                qsTr("I am honored to have served with the noblest knights history has ever seen.")];
    };
    this.getName = function()
    {
        return qsTr("Javier");
    };

}

Constructor.prototype = CO;
var CO_JAVIER = new Constructor();
