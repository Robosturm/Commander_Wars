var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(5);
        co.setSuperpowerStars(5);
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
                audio.addMusic("resources/music/cos/sturm.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        CO_STURM.throwMeteor(co, 4);
        audio.clearPlayList();
        CO_STURM.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        CO_STURM.throwMeteor(co, 8);
        audio.clearPlayList();
        CO_STURM.loadCOMusic(co);
        audio.playRandom();
    };

    this.throwMeteor = function(co, damage)
    {
        // let a meteor fall :D
        var meteorTarget = co.getPlayer().getRockettarget(2, damage);
        // create cool meteor animation :)
        var animation = GameAnimationFactory.createAnimation(meteorTarget.x + 2, meteorTarget.y - 4);
        animation.addSprite("meteor", 0, 0, 2500, 3.0);
        animation.addTweenPosition(Qt.point((meteorTarget.x - 2) * map.getImageSize(), (meteorTarget.y - 2) * map.getImageSize()), 1000);
        animation.addTweenScale(120 / 192, 1000);
        animation.addTweenColor(0, "#FFFFFFFF", "#00FFFFFF", 1000, false, 1200);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 4200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true, 1000);

        var fields = globals.getCircle(0, 2);
        // check all fields we can attack
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + meteorTarget.x;
            var y = fields.at(i).y + meteorTarget.y;
            // check with which weapon we can attack and if we could deal damage with this weapon
            if (map.onMap(x, y))
            {
                var unit = map.getTerrain(x, y).getUnit();
                if (unit !== null)
                {
                    var hp = unit.getHpRounded();
                    if (hp <= damage)
                    {
                        // set hp to very very low
                        unit.setHp(0.001);
                    }
                    else
                    {
                        unit.setHp(hp - damage);
                    }
                }
            }
        }
    }

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BH";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 50;
            case GameEnums.PowerMode_Power:
                return 30;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    return 30;
                }
                break;
        }
        return 15;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 50;
            case GameEnums.PowerMode_Power:
                return 30;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    return 30;
                }
                break;
        }
        return 15;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
        {
            return 0;
        }
        else
        {
            return -999;
        }
    };
}

Constructor.prototype = CO;
var CO_STURM = new Constructor();
