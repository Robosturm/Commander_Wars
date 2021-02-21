var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
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
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(2);
            animation.setEndOfAnimationCall("CO_MEIYO", "postAnimationRanking");

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

    this.postAnimationRanking = function(postAnimation)
    {
        postAnimation.seekBuffer();
        var x = postAnimation.readDataInt32();
        var y = postAnimation.readDataInt32();
        var upgrade = postAnimation.readDataInt32();
        if (map.onMap(x, y))
        {
            var unit = map.getTerrain(x, y).getUnit();
            if (unit !== null)
            {
                for (var i = 0; i < upgrade; i++)
                {
                    UNITRANKINGSYSTEM.increaseRang(unit);
                }
            }
        }
    }

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
            audio.addMusic("resources/music/cos/meiyo.mp3", 76678, 146244);
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
            switch (attacker.getUnitRank())
            {
            case 0:
                return 10;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        case GameEnums.PowerMode_Power:
            switch (attacker.getUnitRank())
            {
            case 0:
                return 10;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                switch (attacker.getUnitRank())
                {
                case 0:
                    return 10;
                case 1:
                    return 20;
                case 2:
                    return 30;
                default:
                    return 50;
                }
            }
            else
            {
                switch (attacker.getUnitRank())
                {
                case 0:
                    return -5;
                case 1:
                    return 2.5;
                case 2:
                    return 5;
                default:
                    return 10;
                }
            }
        }
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            switch (defender.getUnitRank())
            {
            case 0:
                return 10;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        case GameEnums.PowerMode_Power:
            switch (defender.getUnitRank())
            {
            case 0:
                return 10;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                switch (defender.getUnitRank())
                {
                case 0:
                    return 10;
                case 1:
                    return 20;
                case 2:
                    return 30;
                default:
                    return 50;
                }
            }
            else
            {
                switch (defender.getUnitRank())
                {
                case 0:
                    return -5;
                case 1:
                    return 2.5;
                case 2:
                    return 5;
                default:
                    return 10;
                }
            }
        }
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("The Supreme Commander of Golden Sun. A veteran of battles who relies on experienced troops.");
    };
    this.getHits = function(co)
    {
        return qsTr("Veterans");
    };
    this.getMiss = function(co)
    {
        return qsTr("Recruits");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Firepower rises the higher a rank of a unit is. However unexperienced units are weaker.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nUnit Ranks are more effective. Units without a Rank loose firepower.") +
                qsTr("\n\nCO Zone Effect: \nUnit Ranks are way more effective.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Units with a higher rank have even higher defence and offense.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Training Camp");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Units with a higher rank have even higher defence and offense. All units gain two ranks.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("War Training");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Get those recruits out of my way!"),
                qsTr("An army of veterans awaits you..."),
                qsTr("I know how to train soldiers!"),
                qsTr("An old soldier without wounds is the most dangerous.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("A knight needs to be strong and experienced!"),
                qsTr("The better prepared knight wins."),
                qsTr("Experience always wins.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("You getting better recuit."),
                qsTr("Tsss! Some recruits have one! A catastroph.")];
    };
    this.getName = function()
    {
        return qsTr("Meiyo");
    };
}

Constructor.prototype = CO;
var CO_MEIYO = new Constructor();
