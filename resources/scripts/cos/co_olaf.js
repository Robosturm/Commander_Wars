var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
        return ["+alt", "+alt2", "+alt3", "+alt4"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
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
            audio.addMusic("resources/music/cos/olaf.mp3", 2618, 59888);
            break;
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);
        map.getGameRules().changeWeather("WEATHER_SNOW", map.getPlayerCount() * 1);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        map.getGameRules().changeWeather("WEATHER_SNOW", map.getPlayerCount() * 1);
        powerNameAnimation.queueAnimation(animation2);
        CO_OLAF.olafDamage(co, 2, animation2, map);
    };

    this.olafDamage = function(co, value, animation2, map)
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

                    animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                    var delay = globals.randInt(135, 265);
                    if (animations.length < 5)
                    {
                        delay *= i;
                    }
                    animation.setSound("power4.wav", 1, delay);
                    if (animations.length < 5)
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        animation2.queueAnimation(animation);
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BM";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (map !== null)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
                {
                    // apply snow buff :)
                    return 60;
                }
                else
                {
                    return 10;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
                    {
                        // apply snow buff :)
                        return 60;
                    }
                    return 10;
                }
                break;
            }
        }
        else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            return 10;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };

    this.getWeatherImmune = function(co, map)
    {
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
        {
            return true;
        }
        return false;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("He may be a pompous braggart, but his tactical prowess has earned him the respect of his peers and the admiration of his people. Old Man Winter");
    };
    this.getHits = function(co)
    {
        return qsTr("Warm boots");
    };
    this.getMiss = function(co)
    {
        return qsTr("Rain clouds");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Winter poses no problem for Olaf or his troops. Snow causes his firepower to rise, and his troops can move through it without any penalties.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nWinter poses no problem for Olaf or his troops.") +
                qsTr("\n\nCO Zone Effect: \nFirepower is increased by during Snow.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Causes snow to fall for one day, causing his firepower to rise.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Blizzard");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("A mighty blizzard causes two HP of damage to all enemy troops. The snow will also cause his firepower to rise for one day.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Winter Fury");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Oho ho ho.  Do you think your pitiful troops can stand the cold?"),
                qsTr("Neither man nor machine can withstand the fury of nature!"),
                qsTr("You're going to regret challenging me!"),
                qsTr("Let the winds of war bring snow!"),
                qsTr("I'll bury you!"),
                qsTr("Your weapons are powerless before the might of nature!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Olaf's troops know no match!"),
                qsTr("I won! ...That is...we won!"),
                qsTr("In the end, Olaf stands victorious!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Unbelievable! We've been forced to withdraw? What's going on?"),
                qsTr("Next time, I will give them a display of true might! Mark my words!")];
    };
    this.getName = function()
    {
        return qsTr("Olaf");
    };
    this.getAiUsePower = function(co, powerSurplus, turnMode)
    {
        // scop spam
        if (co.canUseSuperpower())
        {
            return GameEnums.PowerMode_Superpower;
        }
        return GameEnums.PowerMode_Off;
    };
}

Constructor.prototype = CO;
var CO_OLAF = new Constructor();
