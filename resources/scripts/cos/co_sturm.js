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
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(false);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_STURM.throwMeteor(co, 4, powerNameAnimation);
        audio.clearPlayList();
        CO_STURM.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(true);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_STURM.throwMeteor(co, 8, powerNameAnimation);
        audio.clearPlayList();
        CO_STURM.loadCOMusic(co);
        audio.playRandom();
    };

    this.throwMeteor = function(co, damage, powerNameAnimation)
    {
        // let a meteor fall :D
        var meteorTarget = co.getPlayer().getRockettarget(2, damage);
        // create cool meteor animation :)
        var animation = GameAnimationFactory.createAnimation(meteorTarget.x + 2, meteorTarget.y - 4);
        animation.addSprite("meteor", 0, 0, 2500, 3.0);
        animation.addTweenPosition(Qt.point((meteorTarget.x - 2) * map.getImageSize(), (meteorTarget.y - 2) * map.getImageSize()), 1000);
        animation.addTweenScale(120 / 192, 1000);
        animation.addTweenColor(0, "#FFFFFFFF", "#00FFFFFF", 1000, false, 1200);
        powerNameAnimation.queueAnimation(animation);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 4200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true, 1000);
        powerNameAnimation.queueAnimation(animation2);

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
    this.getMovementcostModifier = function(co, unit, posX, posY)
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

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("The original commander of the Black Hole army. A mysterious invader from another world. Mastermind of the Cosmo and Macro wars. Enigma");
    };
    this.getHits = function()
    {
        return qsTr("Plotting invasions");
    };
    this.getMiss = function()
    {
        return qsTr("Peace");
    };
    this.getCODescription = function()
    {
        return qsTr("His troops are not affected by terrain only snow can stop him.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("A small Meteor fall from space and deal 4 HP of damage to all affected units.");
    };
    this.getPowerName = function()
    {
        return qsTr("Meteor");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Pulls a giant meteor from space, which deals 8 HP of damage to all affected units. Firepower and defense is increased.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Meteor Strike");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Prepare to embrace darkness!"),
                qsTr("You will tremble before my power!"),
                qsTr("Fear is all you have left..."),
                qsTr("You shall not survive!"),
                qsTr("Burning earth!!!"),
                qsTr("Such power... I regret crushing it.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Who would have thought you could oppose me?"),
                qsTr("My name is Sturm. Hear it and tremble."),
                qsTr("This is but a taste of my power!")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Gwaaaaaaaaaahhhh! I underestimated the strength of these worms!"),
                qsTr("NOOOOOOOOOOOOOOOOOOO!!!")];
    };
    this.getName = function()
    {
        return qsTr("Sturm");
    };
}

Constructor.prototype = CO;
var CO_STURM = new Constructor();
