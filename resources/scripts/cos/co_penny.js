var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
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
        // cool new random weather
        var rules = map.getGameRules();
        var count = rules.getWeatherCount();
        // weather 0 is sun we don't want sun
        var newWeather = globals.randInt(1, count - 1);
        rules.changeWeather(rules.getWeather(newWeather).getWeatherId(), map.getPlayerCount() * 2);
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);

        var rules = map.getGameRules();
        var count = rules.getWeatherCount();
        // weather 0 is sun we don't want sun
        var newWeather = globals.randInt(1, count - 1);
        rules.changeWeather(rules.getWeather(newWeather).getWeatherId(), map.getPlayerCount() * 2);
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
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
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "DM";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
        if (typeof map !== 'undefined')
        {
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                    {
                        return 45;
                    }
                    return 30;
                case GameEnums.PowerMode_Power:
                    if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                    {
                        // apply sandstorm buff :)
                        return 25;
                    }
                    return 10;
                default:
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) ||
                            co.getPowerMode() > GameEnums.PowerMode_Off)
                    {
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
                                       defender, defPosX, defPosY, isDefender, action)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY)
    {
        if (typeof map !== 'undefined')
        {
            if (map.getGameRules().getCurrentWeather() !== null &&
                map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM" &&
                unit.getBaseMaxRange() > 1)
            {
                if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
                {
                    return 3;
                }
            }
        }
        return 0;
    };

    this.getWeatherImmune = function(co)
    {
        return true;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
            {
                return 2;
            }
        }
        return 0;
    };

    this.getVisionrangeModifier = function(co, unit, posX, posY)
    {
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_RAIN")
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return 3;
            }
        }
        return 0;
    };

    this.getPerfectVision = function(co)
    {
        if (map.getGameRules().getCurrentWeather() !== null &&
            map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_RAIN")
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return true;
            }
        }
        return false;
    };
    this.getAiCoUnitBonus = function(co, unit)
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
        return qsTr("\nGlobal Effect: \nHer troops are immune to all weather effects.") +
               qsTr("\n\nCO Zone Effect: \n10% increased firepower and defense.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Changes the weather to a random one.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Stormfront");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Changes the weather to a random one and her troops get a buff depending on the new weather.");
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
