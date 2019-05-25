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

        audio.clearPlayList();
        CO_PENNY.loadCOMusic(co);
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
        powerNameAnimation.queueAnimation(animation2);

        var rules = map.getGameRules();
        var count = rules.getWeatherCount();
        // weather 0 is sun we don't want sun
        var newWeather = globals.randInt(1, count - 1);
        rules.changeWeather(rules.getWeather(newWeather).getWeatherId(), map.getPlayerCount() * 2);

        audio.clearPlayList();
        CO_PENNY.loadCOMusic(co);
        audio.playRandom();
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/penny.mp3")
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
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                {
                    return 45
                }
                return 30;
            case GameEnums.PowerMode_Power:
                break;
            default:                
                break;
        }
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
        {
            // apply sandstorm buff :)
            return 15;
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY)
    {
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM" &&
            unit.getBaseMaxRange() > 1)
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
            {
                return 3;
            }
            return 1;
        }
        return 0;
    };
    this.getMovementcostModifier = function(co, unit, posX, posY)
    {
        if (unit.getOwner() === co.getOwner())
        {
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
            {
                // apply snow buff :)
                return -1;
            }
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
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
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
            {
                return 3;
            }
            return 1;
        }
    };

    this.getPerfectVision = function(co)
    {
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_RAIN")
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
            {
                return true;
            }
        }
        return false;
    }

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Yougest child of Caulder. She is still too young ro know right from wrong.");
    };
    this.getHits = function()
    {
        return qsTr("Mr. Bear");
    };
    this.getMiss = function()
    {
        return qsTr("Broken \"Toys\"");
    };
    this.getCODescription = function()
    {
        return qsTr("Her troops are immune to all weather effects");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Changes the weather to a random one.");
    };
    this.getPowerName = function()
    {
        return qsTr("Stormfront");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Changes the weather to a random one and her troops get a buff depending on the new weather.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Weatherchaos");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Tee he he!!!"),
                qsTr("Nice weather we're having"),
                qsTr("Giggle")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Daddy will be pleased!"),
                qsTr("Wow! I'm getting the hang of this!"),
                qsTr("Tee he he!!!")];
    };
    this.getDefeatSentences = function()
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
