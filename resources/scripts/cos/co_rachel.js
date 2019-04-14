var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/rachel.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(false);
        dialogAnimation.queueAnimation(powerNameAnimation);

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
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_RACHEL.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(true);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var ret = CO_RACHEL.throwRocket(co, 3, GameEnums.RocketTarget_HpLowMoney, powerNameAnimation);
        ret = CO_RACHEL.throwRocket(co, 3, GameEnums.RocketTarget_HpHighMoney, ret);
        CO_RACHEL.throwRocket(co, 3, GameEnums.RocketTarget_Money, ret);
        audio.clearPlayList();
        CO_RACHEL.loadCOMusic(co);
        audio.playRandom();
    };

    this.throwRocket = function(co, damage, targetType, animation2)
    {
        // let a rocket fall :D
        var rocketTarget = co.getPlayer().getRockettarget(2, damage, 1.0, targetType);
        
        var animation = GameAnimationFactory.createAnimation(rocketTarget.x - 2, rocketTarget.y - 2 - 1);
        animation.addSprite("explosion+silo", -map.getImageSize() / 2, 0, 0, 1.5, 0);
        animation2.queueAnimation(animation);
        audio.playSound("explosion+land.wav");

        var fields = globals.getCircle(0, 2);
        // check all fields we can attack
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + rocketTarget.x;
            var y = fields.at(i).y + rocketTarget.y;
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
        return animation;
    }

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    return 10;
                }
                break;
        }
        return 0;
    };
    this.getBonusLuck = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                break;
            case GameEnums.PowerMode_Power:
                return 40;
            default:
                break;
        }
        return 0;
    };

    this.getRepairBonus = function(co, unit, posX, posY)
    {
        return 1;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Brings a breath of fresh air to her troops. Strives to follow in the footsteps of her older sister, Nell. Led the Allied Nations during the Omega war. Rocket Girl");
    };
    this.getHits = function()
    {
        return qsTr("Hard Work");
    };
    this.getMiss = function()
    {
        return qsTr("Excuses");
    };
    this.getCODescription = function()
    {
        return qsTr("Her troops are quite hardworking, and they increase repairs by one on properties.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Has a chance to strike with more damage than expected. Lucky!");
    };
    this.getPowerName = function()
    {
        return qsTr("Lucky Lass");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Launches three missiles from Orange Star HQ in Omega Land.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Covering Fire");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("I will protect this land to the end!"),
                qsTr("This time, I'm serious!"),
                qsTr("You don't have a chance!"),
                qsTr("I'm going to send you packing!"),
                qsTr("Don't pick a fight with me!"),
                qsTr("Finally, some real competition!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Another one down... Who's next?"),
                qsTr("I never give up."),
                qsTr("Don't take me lightly just because I'm cute!")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("You beat me! Hmmm... Not bad!"),
                qsTr("You've got to be kidding me!")];
    };
    this.getName = function()
    {
        return qsTr("Rachel");
    };
}

Constructor.prototype = CO;
var CO_RACHEL = new Constructor();
