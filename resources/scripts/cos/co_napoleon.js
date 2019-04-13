var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
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
                audio.addMusic("resources/music/cos/napoleon.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power5", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power5", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        audio.clearPlayList();
        CO_NAPOLEON.loadCOMusic(co);
        audio.playRandom();
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:

                break;
            case GameEnums.PowerMode_Power:

                break;
            default:
                break;
        }
    };

    this.activateSuperpower = function(co)
    {
        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
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
        audio.clearPlayList();
        CO_NAPOLEON.loadCOMusic(co);
        audio.playRandom();
    };

    
    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var terrainDefense = map.getTerrain(atkPosX, atkPosY).getDefense(attacker);
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
            return terrainDefense * 10;
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY)))
            {
                return 0;
            }
            break;
        }
        return 0;
    };

    this.getAttackHpBonus = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
            return 4;
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            break;
        }
        return 0;
    };

    this.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
            return 0;
        case GameEnums.PowerMode_Power:
            var defHp = defender.getHp() * 10;
            if (damage  > defHp / 2)
            {
                return damage - defHp / 2;
            }
            return 0;
        default:
            break;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY))
                    {
                        return 45
                    }
                    else
                    {
                        return 40;
                    }
                }
                break;
            case GameEnums.PowerMode_Power:
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY))
                    {
                        return 30
                    }
                    else
                    {
                        return 20;
                    }
                }
                break;
        }
        if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY))
        {
            return 30
        }
        else
        {
            return 15;
        }
    };

    this.getCOArmy = function()
    {
        return "MA";
    };
}

Constructor.prototype = CO;
var CO_NAPOLEON = new Constructor();
