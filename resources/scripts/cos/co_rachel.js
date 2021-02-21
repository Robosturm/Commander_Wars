var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
    };

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
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/rachel.mp3", 2220, 63662);
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
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
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var ret = CO_RACHEL.throwRocket(co, 3, GameEnums.RocketTarget_HpLowMoney, powerNameAnimation, 0);
        ret = CO_RACHEL.throwRocket(co, 3, GameEnums.RocketTarget_HpHighMoney, ret, 1);
        CO_RACHEL.throwRocket(co, 3, GameEnums.RocketTarget_Money, ret, 2);
    };

    this.throwRocket = function(co, damage, targetType, animation2, index)
    {
        // let a rocket fall :D
        var rocketTarget = co.getOwner().getRockettarget(2, damage, 1.2, targetType);
        
        var animation = GameAnimationFactory.createAnimation(rocketTarget.x - 2, rocketTarget.y - 2 - 1);
        animation.addSprite("explosion+silo", -map.getImageSize() / 2, 0, 0, 1.5, 0);
        animation.setSound("missle_explosion.wav", 1);
        animation.setEndOfAnimationCall("CO_RACHEL", "postAnimationThrowRocket" + index.toString());
        animation2.queueAnimation(animation);
        CO_RACHEL.postAnimationThrowRocketTarget[index] = rocketTarget;
        CO_RACHEL.postAnimationThrowRocketDamage[index] = damage;
        return animation;
    };
    this.postAnimationThrowRocketTarget = [null, null, null];
    this.postAnimationThrowRocketDamage = [0, 0, 0];

    this.postAnimationThrowRocket0 = function( animation)
    {
        CO_RACHEL.postAnimationThrowRocket(0);
    };
    this.postAnimationThrowRocket1 = function( animation)
    {
        CO_RACHEL.postAnimationThrowRocket(1);
    };
    this.postAnimationThrowRocket2 = function( animation)
    {
        CO_RACHEL.postAnimationThrowRocket(2);
    };
    this.postAnimationThrowRocket = function(index)
    {
        var rocketTarget = CO_RACHEL.postAnimationThrowRocketTarget[index];
        var damage = CO_RACHEL.postAnimationThrowRocketDamage[index];
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
        fields.remove();
        CO_RACHEL.postAnimationThrowRocketTarget[index] = null;
        CO_RACHEL.postAnimationThrowRocketDamage[index] = 0;
    };

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
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 20;
                }
                break;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isDefender)
        {
            if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                    co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return 20;
            }
            return 0;
        };
    this.getBonusLuck = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
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
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Brings a breath of fresh air to her troops. Strives to follow in the footsteps of her older sister, Nell. Led the Allied Nations during the Omega war. Rocket Girl");
    };
    this.getHits = function(co)
    {
        return qsTr("Hard Work");
    };
    this.getMiss = function(co)
    {
        return qsTr("Excuses");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Her troops are quite hardworking, and they increase repairs by one on properties.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nUnits have increased repairs by one on properties.") +
               qsTr("\n\nCO Zone Effect: \nUnits have increased firepower.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Has a chance to strike with more damage than expected. Lucky!");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Lucky Lass");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Launches three missiles from Orange Star HQ in Omega Land.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Covering Fire");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I will protect this land to the end!"),
                qsTr("This time, I'm serious!"),
                qsTr("You don't have a chance!"),
                qsTr("I'm going to send you packing!"),
                qsTr("Don't pick a fight with me!"),
                qsTr("Finally, some real competition!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Another one down... Who's next?"),
                qsTr("I never give up."),
                qsTr("Don't take me lightly just because I'm cute!")];
    };
    this.getDefeatSentences = function(co)
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
