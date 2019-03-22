var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
    };

    this.activatePower = function(co)
    {
        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        map.getGameRules().changeWeather("WEATHER_SNOW", map.getPlayerCount() * 2);
        audio.clearPlayList();
        audio.addMusic("resources/music/cos/power.mp3");
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        map.getGameRules().changeWeather("WEATHER_SNOW", map.getPlayerCount() * 2);
        CO_OLAF.olafDamage(co, 2);
        audio.clearPlayList();
        audio.addMusic("resources/music/cos/superpower.mp3");
        audio.playRandom();
    };

    this.olafDamage = function(co, value)
    {
        var player = co.getPlayer();
        var counter = 0;
        var playerCounter = map.getPlayerCount();
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

                    var hp = unit.getHpRounded();
                    if (hp <= value)
                    {
                        // set hp to very very low
                        unit.setHp(0.001);
                    }
                    else
                    {
                        unit.setHp(hp - value);
                    }
                }
                units.remove();
            }
        }
    };

    this.startOfTurn = function(co)
    {
        audio.addMusic("resources/music/cos/olaf.mp3")
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "BM";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
                {
                    // apply snow buff :)
                    return 40;
                }
                else
                {
                    return 20;
                }
            case GameEnums.PowerMode_Power:
                if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
                {
                    // apply snow buff :)
                    return 40;
                }
                else
                {
                    return 20;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
                    {
                        // apply snow buff :)
                        return 20;
                    }
                    return 10;
                }
                break;
        }
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
        {
            // apply snow buff :)
            return 10;
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
        {
            // apply snow buff :)
            return -1;
        }
        else
        {
            return 0;
        }
    };
}

Constructor.prototype = CO;
var CO_OLAF = new Constructor();
