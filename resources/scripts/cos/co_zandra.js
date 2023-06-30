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
                audio.addMusic("resources/music/cos/zandra.mp3", 59, 57817);
                break;
            }
        }
    };

    this.sandstormDamage = 2;

    this.powerSandstormBonus = 55;
    this.powerSandstormRangeBonus = 1;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;

    this.d2dSandStormBonus = 0;

    this.d2dCoZoneSandStormBonus = 55;
    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);
        map.getGameRules().changeWeather("WEATHER_SANDSTORM", map.getPlayerCount() * 1);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        map.getGameRules().changeWeather("WEATHER_SANDSTORM", map.getPlayerCount() * 1);
        powerNameAnimation.queueAnimation(animation2);
        CO_ZANDRA.zandraDamage(co, CO_ZANDRA.sandstormDamage, animation2, map);
    };

    this.zandraDamage = function(co, value, animation2, map)
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
                var size = units.size();
                for (i = 0; i < size; i++)
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
        return "BD";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (map !== null)
            {
                var hasSandstorm = map.getGameRules().getCurrentWeather() !== null &&
                                   map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM";
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                case GameEnums.PowerMode_Power:
                    if (hasSandstorm)
                    {
                        return CO_ZANDRA.powerSandstormBonus;
                    }
                    return CO_ZANDRA.powerOffBonus;
                default:
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        if (hasSandstorm)
                        {
                            return CO_ZANDRA.d2dCoZoneSandStormBonus;
                        }
                        return CO_ZANDRA.d2dCoZoneOffBonus;
                    }
                    else if (hasSandstorm)
                    {
                        return CO_ZANDRA.d2dSandStormBonus;
                    }
                    break;
                }
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_ZANDRA.d2dCoZoneOffBonus;
            }
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_ZANDRA.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_ZANDRA.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (map !== null)
            {
                if (map.getGameRules().getCurrentWeather() !== null &&
                        map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                {
                    if (unit.getBaseMaxRange() > 1)
                    {
                        switch (co.getPowerMode())
                        {
                        case GameEnums.PowerMode_Tagpower:
                        case GameEnums.PowerMode_Superpower:
                        case GameEnums.PowerMode_Power:
                            return CO_ZANDRA.sandstormRangeBonus;
                        default:
                            return 0;
                        }
                    }
                }
            }
        }
        return 0;
    };

    this.getWeatherImmune = function(co, map)
    {
        if (CO.isActive(co))
        {
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
            {
                return true;
            }
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
        return qsTr("A youthful CO that will protect her village at all costs. She is skilled in dealing with sandstorms.");
    };
    this.getHits = function(co)
    {
        return qsTr("Coconuts");
    };
    this.getMiss = function(co)
    {
        return qsTr("Seafood");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Zandra's units are unaffected by sandstorms and fight better during them.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nGlobal Effect: \nZandra's units are unaffected by sandstorms and gain +%0% firepower during a sandstorm.") +
                   qsTr("\n\nCO Zone Effect: \nZandra's units gain +%1% firepower during a sandstorm and +%2% firepower otherwise. All her units gain +%3% defence.");
        text = replaceTextArgs(text, [CO_ZANDRA.d2dSandStormBonus, CO_ZANDRA.d2dCoZoneSandStormBonus, CO_ZANDRA.d2dCoZoneOffBonus, CO_ZANDRA.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Changes the weather to a Sandstorm. Zandra's indirect units gain +%0 range. Her units gain +%2% firepower and +%3% defence. Her units gain a total of +%1% firepower while the weather is a Sandstorm.");
        text = replaceTextArgs(text, [CO_ZANDRA.powerSandstormRangeBonus , CO_ZANDRA.powerSandstormBonus, CO_ZANDRA.powerOffBonus, CO_ZANDRA.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Sand Shift");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("A massive gust front changes the weather to a Sandstorm and deals -%2 HP of damage to all enemy units. Zandra's indirect units gain +%0 range. Her units gain +%3% firepower and +%4% defence. Her units gain a total of +%1% firepower while the weather is a Sandstorm.");
        text = replaceTextArgs(text, [CO_ZANDRA.powerSandstormRangeBonus , CO_ZANDRA.powerSandstormBonus, CO_ZANDRA.sandstormDamage, CO_ZANDRA.powerOffBonus, CO_ZANDRA.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Desert Gale");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("It's high time that you felt the wrath of the desert winds."),
                qsTr("I feel the power coming in, on the far winds!"),
                qsTr("You'll regret challenging me."),
                qsTr("Come, father of the four winds!"),
                qsTr("Your eyes will fill with sand, as you scan this wasted land..."),
                qsTr("Troops, attack now, before the winds halt their blessing!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Everyone, hurry back to the village. Our job here is done."),
                qsTr("These sacred lands will be protected for as long as I stand."),
                qsTr("Thank you, Uncle, for giving me the strength I needed.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I only lost because of the snow around here."),
                qsTr("What? I can't lose. Not while the globe is heating up!")];
    };
    this.getName = function()
    {
        return qsTr("Zandra");
    };
}

Constructor.prototype = CO;
var CO_ZANDRA = new Constructor();
