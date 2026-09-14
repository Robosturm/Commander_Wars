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
                audio.addMusic("resources/music/cos/power.ogg", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.ogg", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/zandra.ogg", 59, 57817);
                break;
            }
        }
    };

    this.superPowerSandstormDamage = 2;

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
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, GameEnums.PowerMode_Power, map);

        var flashBuilder = new FLASH_ANIMATION_BUILDER(map);
        dialogBuilder.queueAnimationBuilder(flashBuilder);

        dialogBuilder.displayAnimation();

        map.getGameRules().changeWeather("WEATHER_SANDSTORM", map.getPlayerCount() * 1);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, powerMode, map);

        var flashBuilder = new FLASH_ANIMATION_BUILDER(map);
        dialogBuilder.queueAnimationBuilder(flashBuilder);

        CO_ZANDRA.zandraDamage(co, CO_ZANDRA.superPowerSandstormDamage, dialogBuilder, map);

        dialogBuilder.displayAnimation();

        map.getGameRules().changeWeather("WEATHER_SANDSTORM", map.getPlayerCount() * 1);
    };

    this.zandraDamage = function(co, value, parentBuilder, map)
    {
        var enemyUnitsBuilder = new ENEMY_UNITS_ANIMATION_BUILDER(co, map);
        enemyUnitsBuilder.setSound("power4.wav");
        enemyUnitsBuilder.setSprite("power4");
        enemyUnitsBuilder.setPerAnimationFunction((unit, animation, map) => 
            {
                animation.writeDataInt32(unit.getX());
                animation.writeDataInt32(unit.getY());
                animation.writeDataInt32(value);
                animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
            }
        );
        parentBuilder.queueAnimationBuilder(enemyUnitsBuilder);
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
                        if (map === null ||
                            (map !== null && map.getGameRules().getCoGlobalD2D()))
                        {
                            return CO_ZANDRA.d2dSandStormBonus;
                        }
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
                            return CO_ZANDRA.powerSandstormRangeBonus;
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
    this.getLongCODescription = function(co, map)
    {
        var values = [0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_ZANDRA.d2dSandStormBonus];
        }
        var text = qsTr("\nGlobal Effect: \nZandra's units are unaffected by sandstorms and gain +%0% firepower during a sandstorm.") +
                   qsTr("\n\nCO Zone Effect: \nZandra's units gain +%1% firepower during a sandstorm and +%2% firepower otherwise. All her units gain +%3% defence.");
        text = replaceTextArgs(text, [values[0], CO_ZANDRA.d2dCoZoneSandStormBonus, CO_ZANDRA.d2dCoZoneOffBonus, CO_ZANDRA.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Changes the weather to a Sandstorm. During this power, while the weather is a Sandstorm, Zandra's indirect units gain +%0 range. Her units gain +%2% firepower and +%3% defence. Her units gain a total of +%1% firepower while the weather is a Sandstorm.");
        text = replaceTextArgs(text, [CO_ZANDRA.powerSandstormRangeBonus , CO_ZANDRA.powerSandstormBonus, CO_ZANDRA.powerOffBonus, CO_ZANDRA.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Sand Shift");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("A massive gust front changes the weather to a Sandstorm and deals -%2 HP of damage to all enemy units. During this power, while the weather is a Sandstorm, Zandra's indirect units gain +%0 range. Her units gain +%3% firepower and +%4% defence. Her units gain a total of +%1% firepower while the weather is a Sandstorm.");
        text = replaceTextArgs(text, [CO_ZANDRA.powerSandstormRangeBonus , CO_ZANDRA.powerSandstormBonus, CO_ZANDRA.superPowerSandstormDamage, CO_ZANDRA.powerOffBonus, CO_ZANDRA.powerDefBonus]);
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
