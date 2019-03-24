var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(5);
    };

    this.activatePower = function(co)
    {
        CO_ROBOSTURM.power(co, 0);
        audio.clearPlayList();
        audio.addMusic("resources/music/cos/bh_power.mp3");
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        CO_ROBOSTURM.power(co, 2);
        audio.clearPlayList();
        audio.addMusic("resources/music/cos/bh_superpower.mp3");
        audio.playRandom();
    };

    this.power = function(co, value)
    {

        var player = co.getPlayer();
        var units = player.getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();

        var playerCounter = map.getPlayerCount();
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {

                units = enemyPlayer.getUnits();
                units.randomize();
                for (i = 0; i < units.size(); i++)
                {
                    unit = units.at(i);

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
                    // reduce ammo
                    if (unit.getMaxAmmo2() > 0)
                    {
                        unit.reduceAmmo2(unit.getAmmo2() / 2);
                    }
                    if (unit.getMaxAmmo1() > 0)
                    {
                        unit.reduceAmmo1(unit.getAmmo1() / 2);
                    }

                    animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                    if (animations.length < 5)
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                        animations[counter].queueAnimation(animation);
                        animations[counter] = animation;
                        counter++;
                        if (counter >= animations.length)
                        {
                            counter = 0;
                        }
                    }
                }
                units.remove();
            }
        }
    };

    this.startOfTurn = function(co)
    {
        audio.addMusic("resources/music/cos/robosturm.mp3")
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 0;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    return 0;
                }
                break;
        }
        return -20;
    };



    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 80;
            case GameEnums.PowerMode_Power:
                return 60;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    return 40;
                }
                break;
        }
        return 20;
    };

    this.getCOArmy = function()
    {
        return "MA";
    };
    this.getMovementPointModifier = function(co, unit)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 2;
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
        {
            return 3;
        }
        return 1;
    };
}

Constructor.prototype = CO;
var CO_ROBOSTURM = new Constructor();
