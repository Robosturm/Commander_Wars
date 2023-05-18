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
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(2);
            animation.setEndOfAnimationCall("CO_MEIYO", "postAnimationRanking");
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

    this.postAnimationRanking = function(postAnimation, map)
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

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
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

    this.powerSoldier = 10;
    this.powerExperienced = 20;
    this.powerVeteran = 30;
    this.powerElite = 50;

    this.d2dSoldier = -5;
    this.d2dExperienced = 3;
    this.d2dVeteran = 5;
    this.d2dElite = 10;

    this.d2dCoZoneSoldier = 10;
    this.d2dCoZoneExperienced = 20;
    this.d2dCoZoneVeteran = 30;
    this.d2dCoZoneElite = 50;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                switch (defender.getUnitRank())
                {
                case 0:
                    return CO_MEIYO.powerSoldier;
                case 1:
                    return CO_MEIYO.powerExperienced;
                case 2:
                    return CO_MEIYO.powerVeteran;
                default:
                    return CO_MEIYO.powerElite;
                }
            default:
                if (CO.getGlobalZone())
                {
                    switch (defender.getUnitRank())
                    {
                    case 0:
                        return CO_MEIYO.d2dSoldier;
                    case 1:
                        return CO_MEIYO.d2dExperienced;
                    case 2:
                        return CO_MEIYO.d2dVeteran;
                    default:
                        return CO_MEIYO.d2dElite;
                    }
                }
                else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    switch (defender.getUnitRank())
                    {
                    case 0:
                        return CO_MEIYO.d2dCoZoneSoldier;
                    case 1:
                        return CO_MEIYO.d2dCoZoneExperienced;
                    case 2:
                        return CO_MEIYO.d2dCoZoneVeteran;
                    default:
                        return CO_MEIYO.d2dCoZoneElite;
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
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                switch (defender.getUnitRank())
                {
                case 0:
                    return CO_MEIYO.powerSoldier;
                case 1:
                    return CO_MEIYO.powerExperienced;
                case 2:
                    return CO_MEIYO.powerVeteran;
                default:
                    return CO_MEIYO.powerElite;
                }
            default:
                if (CO.getGlobalZone())
                {
                    switch (defender.getUnitRank())
                    {
                    case 0:
                        return CO_MEIYO.d2dSoldier;
                    case 1:
                        return CO_MEIYO.d2dExperienced;
                    case 2:
                        return CO_MEIYO.d2dVeteran;
                    default:
                        return CO_MEIYO.d2dElite;
                    }
                }
                else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    switch (defender.getUnitRank())
                    {
                    case 0:
                        return CO_MEIYO.d2dCoZoneSoldier;
                    case 1:
                        return CO_MEIYO.d2dCoZoneExperienced;
                    case 2:
                        return CO_MEIYO.d2dCoZoneVeteran;
                    default:
                        return CO_MEIYO.d2dCoZoneElite;
                    }
                }
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
        let text = qsTr("\nGlobal Effect: \nUnit Ranks are more effective.\n" +
                    "Soldier have %0% firepower and defence.\n" +
                    "Experienced have %1% firepower and defence.\n" +
                    "Veteran have %2% firepower and defence.\n" +
                    "Elite have %3% firepower and defence.\n" +
                    "\n\nCO Zone Effect: \nUnit Ranks are way more effective.\n" +
                    "Soldier have %4% firepower and defence.\n" +
                    "Experienced have %5% firepower and defence.\n" +
                    "Veteran have %6% firepower and defence.\n" +
                    "Elite have %7% firepower and defence.\n");
        text = replaceTextArgs(text, [CO_MEIYO.d2dSoldier, CO_MEIYO.d2dExperienced, CO_MEIYO.d2dVeteran, CO_MEIYO.d2dElite,
                                      CO_MEIYO.d2dCoZoneSoldier, CO_MEIYO.d2dCoZoneExperienced, CO_MEIYO.d2dCoZoneVeteran, CO_MEIYO.d2dCoZoneElite]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        let text = qsTr("Units with a higher rank have even higher defence and offense.\n" +
                    "Soldier have %0% firepower and defence.\n" +
                    "Experienced have %1% firepower and defence.\n" +
                    "Veteran have %2% firepower and defence.\n" +
                    "Elite have %3% firepower and defence.");
        text = replaceTextArgs(text, [CO_MEIYO.powerSoldier, CO_MEIYO.powerExperienced, CO_MEIYO.powerVeteran, CO_MEIYO.powerElite]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Training Camp");
    };
    this.getSuperPowerDescription = function(co)
    {
        let text = qsTr("Units with a higher rank have even higher defence and offense. All units gain two ranks.\n" +
                    "Soldier have %0% firepower and defence.\n" +
                    "Experienced have %1% firepower and defence.\n" +
                    "Veteran have %2% firepower and defence.\n" +
                    "Elite have %3% firepower and defence.");
        text = replaceTextArgs(text, [CO_MEIYO.powerSoldier, CO_MEIYO.powerExperienced, CO_MEIYO.powerVeteran, CO_MEIYO.powerElite]);
        return text;
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
        return [qsTr("You getting better recruit."),
                qsTr("Tsss! Some recruits have one! A catastrophe.")];
    };
    this.getName = function()
    {
        return qsTr("Meiyo");
    };
}

Constructor.prototype = CO;
var CO_MEIYO = new Constructor();
