var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2", "+alt3", "+alt4"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
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
                audio.addMusic("resources/music/cos/olaf.mp3", 2618, 59888);
                break;
            }
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
        map.getGameRules().changeWeather("WEATHER_SNOW", map.getPlayerCount() * CO_OLAF.powerSnowDays);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        map.getGameRules().changeWeather("WEATHER_SNOW", map.getPlayerCount() * CO_OLAF.powerSnowDays);
        powerNameAnimation.queueAnimation(animation2);
        CO_OLAF.olafDamage(co, CO_OLAF.superPowerDamage, animation2, map);
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

    this.superPowerDamage = 2;
    this.superPowerOffBonus = 60;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;
    this.powerSnowDays = 1;
    this.d2dOffBonus = 0;
    this.d2dRainMalus = true;
    this.d2dCoZoneOffBonus = 60;
    this.d2dCoZoneBaseOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
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
                        return CO_OLAF.superPowerOffBonus;
                    }
                    else
                    {
                        return CO_OLAF.powerOffBonus;
                    }
                default:
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
                        {
                            return CO_OLAF.d2dCoZoneOffBonus;
                        }
                        return CO_OLAF.d2dCoZoneBaseOffBonus;
                    }
                    else if (map === null ||
                             (map !== null && map.getGameRules().getCoGlobalD2D()))
                    {
                        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
                        {
                            return CO_OLAF.d2dOffBonus;
                        }
                    }
                    break;
                }
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_OLAF.d2dCoZoneBaseOffBonus;
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
                return CO_OLAF.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_OLAF.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };

    this.getWeatherImmune = function(co, map)
    {
        if (CO.isActive(co))
        {
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
            {
                if (co.getPowerMode() > GameEnums.PowerMode_Off)
                {
                    return true;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return true;
                }
            }
        }
        return false;
    };
    this.getMovementcostModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co) && CO_OLAF.d2dRainMalus === true)
        {
            if (map !== null)
            {
                if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_RAIN" &&
                    unit.getOwner() === co.getOwner())
                {
                    var terrain = map.getTerrain(posX, posY);
                    var id = terrain.getID();
                    if ((unit.getUnitType() === GameEnums.UnitType_Air) ||
                        (id !== "STREET" &&
                         id !== "STREET1" &&
                         id !== "SNOW_STREET" &&
                         id !== "BRIDGE" &&
                         id !== "BRIDGE1" &&
                         id !== "BRIDGE2" &&
                         id !== "WASTE_PATH" &&
                         id !== "DESERT_PATH" &&
                         id !== "DESERT_PATH1" &&
                         id !== "TELEPORTTILE" &&
                        terrain.getBuilding() === null))
                    {
                        if (co.getPowerMode() > GameEnums.PowerMode_Off)
                        {
                            return 1;
                        }
                        else if (map === null ||
                                 (map !== null && map.getGameRules().getCoGlobalD2D()))
                        {
                            return 1;
                        }
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
        return qsTr("He may be a pompous braggart, but his tactical prowess has earned him the respect of his peers, the admiration of his people, and the nickname 'Old Man Winter'.");
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
        return qsTr("Winter poses no problem for Olaf or his troops. Snow causes his firepower to rise, and his troops can move through it without any penalties. Rain, however, slows him down.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_OLAF.d2dOffBonus];
        }
        var text = qsTr("\nGlobal Effect: \nOlaf's units have +%0% firepower if the weather is Snow. If the weather is Rain, his units' movement costs are increased by 1 in all terrain but roads and shoals.") +
                qsTr("\n\nCO Zone Effect: \nOlaf's units gain +%2% firepower and +%3% defence. His units gain a total of +%1% firepower if the weather is Snow.");
        text = replaceTextArgs(text, [values[0], CO_OLAF.d2dCoZoneOffBonus, CO_OLAF.d2dCoZoneBaseOffBonus, CO_OLAF.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Changes the weather to Snow. Olaf's units gain +%1% firepower and +%2% defence. His units gain a total of +%0% firepower while the weather is Snow.");
        text = replaceTextArgs(text, [CO_OLAF.superPowerOffBonus, CO_OLAF.powerOffBonus, CO_OLAF.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Blizzard");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("A mighty blizzard changes the weather to Snow and deals -%0 HP of damage to all enemy units. Olaf's units gain +%2% firepower and +%3% defence. His units gain a total of +%1% firepower while the weather is Snow.");
        text = replaceTextArgs(text, [CO_OLAF.superPowerDamage, CO_OLAF.superPowerOffBonus, CO_OLAF.powerOffBonus, CO_OLAF.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Winter Fury");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Oho ho ho. Do you think your pitiful troops can stand the cold?"),
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
