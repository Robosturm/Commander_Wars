var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
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
        // cool new random weather
        var rules = map.getGameRules();
        var count = rules.getWeatherCount();
        // weather 0 is sun we don't want sun
        var newWeather = globals.randInt(1, count - 1);
        rules.changeWeather(rules.getWeather(newWeather).getWeatherId(), map.getPlayerCount() * CO_PENNY.powerDayChanges);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);

        var rules = map.getGameRules();
        var count = rules.getWeatherCount();
        // weather 0 is sun we don't want sun
        var newWeather = globals.randInt(1, count - 1);
        rules.changeWeather(rules.getWeather(newWeather).getWeatherId(), map.getPlayerCount() * CO_PENNY.powerDayChanges);
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0 , 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0 , 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/penny.mp3", 56308, 108812);
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
        return "DM";
    };

    this.superPowerOffBonus = 30;
    this.superPowerFirerangeModifier = 2;
    this.superPowerMovementPointBonus = 2;
    this.superPowerRainVisionBonus = 3;
    this.superPowerMistVisionBonus = 5;
    this.powerDayChanges = 2;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;
    this.d2dCoZoneOffBonus = 10;
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
                    return CO_PENNY.superPowerOffBonus;
                case GameEnums.PowerMode_Power:
                    return CO_PENNY.powerOffBonus;
                default:
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) ||
                        co.getPowerMode() > GameEnums.PowerMode_Off)
                    {
                        return CO_PENNY.d2dCoZoneOffBonus;
                    }
                    break;
                }
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_PENNY.d2dCoZoneOffBonus;
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
                return CO_PENNY.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_PENNY.d2dCoZoneDefBonus;
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
                    map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM" &&
                    unit.getBaseMaxRange() > 1)
                {
                    if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
                    {
                        return CO_PENNY.superPowerFirerangeModifier;
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
            return true;
        }
        return false;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                    co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
                {
                    return CO_PENNY.superPowerMovementPointBonus;
                }
            }
        }
        return 0;
    };

    this.getVisionrangeModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (map.getGameRules().getCurrentWeather() !== null)
            {
                if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                        co.getPowerMode() === GameEnums.PowerMode_Tagpower)
                {
                    var currentWeatherId = map.getGameRules().getCurrentWeather().getWeatherId();
                    if (currentWeatherId === "WEATHER_RAIN")
                    {
                        return CO_PENNY.superPowerRainVisionBonus;
                    }
                    else if (currentWeatherId === "WEATHER_MIST")
                    {
                        return CO_PENNY.superPowerMistVisionBonus;
                    }
                }
            }
        }
        return 0;
    };

    this.getPerfectVision = function(co, map)
    {
        if (CO.isActive(co))
        {
            if (map.getGameRules().getCurrentWeather() !== null)
            {
                var currentWeatherId = map.getGameRules().getCurrentWeather().getWeatherId();
                if (currentWeatherId === "WEATHER_RAIN" ||
                        currentWeatherId === "WEATHER_MIST")
                {
                    if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
                    {
                        return true;
                    }
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
        return qsTr("Youngest child of Caulder. She is still too young to know right from wrong.");
    };
    this.getHits = function(co)
    {
        return qsTr("Mr. Bear");
    };
    this.getMiss = function(co)
    {
        return qsTr("Broken \"Toys\"");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Her troops are immune to all weather effects");
    };
    this.getLongCODescription = function()
    {
        let text = qsTr("\nGlobal Effect: \nHer troops are immune to all weather effects." +
                        "\n\nCO Zone Effect: \n%0% increased firepower and %1% defense.");
        text = replaceTextArgs(text, [CO_PENNY.d2dCoZoneOffBonus, CO_PENNY.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        let text = qsTr("Changes the weather to a random one for %0 days.");
        text = replaceTextArgs(text, [CO_PENNY.powerDayChanges]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Stormfront");
    };
    this.getSuperPowerDescription = function(co)
    {
        let text = qsTr("Changes the weather to a random for %4 days and she gets a firepower boost for this turn.\n" +
                    "During rain she gets additionally %0 improved vision.\n" +
                    "During mist she gets additionally %1 improved vision.\n" +
                    "During snow she gets additionally %2 improved movement.\n" +
                    "During sandstorm she gets additionally %3 improved firerange for her indirects.\n");
        text = replaceTextArgs(text, [CO_PENNY.superPowerRainVisionBonus, CO_PENNY.superPowerMistVisionBonus,
                                      CO_PENNY.superPowerMovementPointBonus, CO_PENNY.superPowerFirerangeModifier, CO_PENNY.powerDayChanges]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Weatherchaos");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Tee he he!!!"),
                qsTr("Nice weather we're having"),
                qsTr("Giggle")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Daddy will be pleased!"),
                qsTr("Wow! I'm getting the hang of this!"),
                qsTr("Tee he he!!!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Mr. Bear we lost our toys."),
                qsTr("You don't get Mr. Bear. He's mine!")];
    };
    this.getName = function()
    {
        return qsTr("Penny");
    };
}

Constructor.prototype = CO;
var CO_PENNY = new Constructor();
