var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(6);
        co.setSuperpowerStars(4);
    };

    this.activatePower = function(co)
    {
        CO_VON_BOLT.throwMeteor(co, 2, 2);
        audio.clearPlayList();
        audio.addMusic("resources/music/cos/bh_power.mp3");
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        CO_VON_BOLT.throwMeteor(co, 3, 3);
        audio.clearPlayList();
        audio.addMusic("resources/music/cos/bh_superpower.mp3");
        audio.playRandom();
    };

    this.throwMeteor = function(co, damage, range)
    {
        // let a meteor fall :D
        var meteorTarget = co.getPlayer().getRockettarget(range, damage);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);


        var fields = globals.getCircle(0, range);
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
					unit.setHasMoved(true);
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

    this.startOfTurn = function(co)
    {
        audio.addMusic("resources/music/cos/von_bolt.mp3")
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "BG";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 20;
            case GameEnums.PowerMode_Power:
                return 20;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    return 20;
                }
                break;
        }
        return 10;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 20;
            case GameEnums.PowerMode_Power:
                return 20;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    return 20;
                }
                break;
        }
        return 10;
    };
}

Constructor.prototype = CO;
var CO_VON_BOLT = new Constructor();
