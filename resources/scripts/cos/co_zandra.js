var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3");
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/zandra.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);
        map.getGameRules().changeWeather("WEATHER_SANDSTORM", map.getPlayerCount() * 2);
        audio.clearPlayList();
        CO_ZANDRA.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        map.getGameRules().changeWeather("WEATHER_SANDSTORM", map.getPlayerCount() * 2);
        powerNameAnimation.queueAnimation(animation2);
        CO_ZANDRA.zandraDamage(co, 2, animation2);
        audio.clearPlayList();
        CO_ZANDRA.loadCOMusic(co);
        audio.playRandom();
    };

    this.zandraDamage = function(co, value, animation2)
    {
        var player = co.getOwner();
        var counter = 0;
        var playerCounter = map.getPlayerCount();
        var animation = null;
        var animations = [];

        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {

                var units = enemyPlayer.getUnits();
                units.randomize();
                for (i = 0; i < units.size(); i++)
                {
                    var unit = units.at(i);

                    animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                    if (animations.length < 5)
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                        animation2.queueAnimation(animation);
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                        animations[counter].queueAnimation(animation);
                        animations[counter] = animation;
                        counter++;
                        if (counter >= animations.length)
                        {
                            counter = 0;
                        }
                    }
                    animation.writeDataInt32(unit.getX());
                    animation.writeDataInt32(unit.getY());
                    animation.writeDataInt32(value);
                    animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
                }
                units.remove();
            }
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
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
            case GameEnums.PowerMode_Superpower:
                if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                {
                    // apply sandstorm buff :)
                    return 55;
                }
                else
                {
                    return 0;
                }
            case GameEnums.PowerMode_Power:
                if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                {
                    // apply sandstorm buff :)
                    return 55;
                }
                else
                {
                    return 0;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                    {
                        // apply sandstorm buff :)
                        return 35;
                    }
                    return 0;
                }
                break;
        }
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
        {
            // apply sandstorm buff :)
            return 25;
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY)
    {
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
        {
            if (unit.getBaseMaxRange() > 1)
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Superpower:
                    return 2;
                case GameEnums.PowerMode_Power:
                    return 2;
                default:
                    return 1;
                }
            }
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A youthful Co that will protect her village at all cost. She is skilled in dealing with sandstorms.");
    };
    this.getHits = function()
    {
        return qsTr("Coconuts");
    };
    this.getMiss = function()
    {
        return qsTr("Seafood");
    };
    this.getCODescription = function()
    {
        return qsTr("Zandra's units are unaffected by sandstorms. They fight even better during them.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Causes sandstorm to fall for two days. Increasing the firerange of indirects by 1 and increasing her firepower.");
    };
    this.getPowerName = function()
    {
        return qsTr("Sand Shift");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Causes sandstorm to fall for two days. Increasing the firerange of indirects by 1 and increasing her firepower. Also deals two hp damage to enemies.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Desert Gale");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("It's high time that you felt the wrath of the desert winds."),
                qsTr("I feel the power coming in, on the far winds."),
                qsTr("You'll regret challenging me."),
                qsTr("Come, father of the four winds!"),
                qsTr("Your eyes will fill with sand, as you scan this wasted land..."),
                qsTr("Troops, attack now, before the winds halt their blessing!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Everyone, hurry back to the village. Our job here is done."),
                qsTr("These sacred lands will be protected for as long as I stand."),
                qsTr("Thank you, Uncle, for giving me the strength I needed.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("I only lost because of the Snow around here."),
                qsTr("What? I can't loose. We have a Global Heating.")];
    };
    this.getName = function()
    {
        return qsTr("Zandra");
    };
}

Constructor.prototype = CO;
var CO_ZANDRA = new Constructor();
