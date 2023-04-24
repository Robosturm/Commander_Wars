var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
        return ["+alt", "+alt2"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(5);
        co.setSuperpowerStars(5);
    };

    this.loadCOMusic = function(co, map)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.mp3", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/sturm.mp3", 4054, 60338);
                break;
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_STURM.throwMeteor(co, 4, powerNameAnimation, map);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_STURM.throwMeteor(co, 8, powerNameAnimation, map);
    };

    this.throwMeteor = function(co, damage, powerNameAnimation, map)
    {
        // let a meteor fall :D
        var meteorTarget = co.getOwner().getRockettarget(2, damage);
        // create cool meteor animation :)
        var animation = GameAnimationFactory.createAnimation(map, meteorTarget.x + 2, meteorTarget.y - 4);
        animation.addSprite("meteor", 0, 0, 2500, 4.0);
        animation.addTweenPosition(Qt.point((meteorTarget.x - 2) * map.getImageSize(), (meteorTarget.y - 2) * map.getImageSize()), 1000);
        animation.addTweenScale(0.65, 1000);
        animation.addTweenColor(0, "#FFFFFFFF", "#00FFFFFF", 1000, false, 1200);
        animation.addSound("meteorFall.wav");
        powerNameAnimation.queueAnimation(animation);
        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 4200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true, 1000);
        animation2.addSound("meteorImpact.wav");
        powerNameAnimation.queueAnimation(animation2);
        animation.setEndOfAnimationCall("CO_STURM", "postAnimationThrowMeteor");
        animation.setStartOfAnimationCall("CO_STURM", "preAnimationThrowMeteor");
        CO_STURM.postAnimationThrowMeteorTarget = meteorTarget;
        CO_STURM.postAnimationThrowMeteorDamage = damage;
    };
    this.preAnimationThrowMeteor = function(animation, map)
    {
        map.centerMap(CO_STURM.postAnimationThrowMeteorTarget.x, CO_STURM.postAnimationThrowMeteorTarget.y);
    };
    this.postAnimationThrowMeteorTarget = null;
    this.postAnimationThrowMeteorDamage = 0;
    this.postAnimationThrowMeteor = function(animation, map)
    {
        var meteorTarget = CO_STURM.postAnimationThrowMeteorTarget;
        var damage = CO_STURM.postAnimationThrowMeteorDamage;
        var fields = globals.getCircle(0, 2);
        // check all target fields
        var size = fields.size();
        for (var i = 0; i < size; i++)
        {
            var x = fields.at(i).x + meteorTarget.x;
            var y = fields.at(i).y + meteorTarget.y;
            // check if the target is on the map
            if (map.onMap(x, y))
            {
                var unit = map.getTerrain(x, y).getUnit();
                if (unit !== null)
                {
                    var hp = unit.getHpRounded();
                    if (hp - damage <= 0.1)
                    {
                        // set hp to very very low
                        unit.setHp(0.1);
                    }
                    else
                    {
                        unit.setHp(hp - damage);
                    }
                }
            }
        }
        CO_STURM.postAnimationThrowMeteorTarget = null;
        CO_STURM.postAnimationThrowMeteorDamage = 0;
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BH";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 50;
            case GameEnums.PowerMode_Power:
                return 30;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 30;
                }
                break;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 50;
            case GameEnums.PowerMode_Power:
                return 30;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return 30;
                }
                break;
        }
        return 0;
    };
    this.getMovementcostModifier = function(co, unit, posX, posY, map)
    {
        if (unit.getOwner() === co.getOwner())
        {
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
            {
                return 0;
            }
            else
            {
                return -999;
            }
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("The original commander of the Black Hole army. A mysterious invader from another world. Mastermind of the Cosmo and Macro wars. Enigma");
    };
    this.getHits = function(co)
    {
        return qsTr("Plotting invasions");
    };
    this.getMiss = function(co)
    {
        return qsTr("Peace");
    };
    this.getCODescription = function(co)
    {
        return qsTr("His troops are not affected by terrain only snow can stop him.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nHis troops are not affected by terrain except snow.") +
               qsTr("\n\nCO Zone Effect: \nUnits have increased firepower and defense.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("A small Meteor fall from space and deal 4 HP of damage to all affected units.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Meteor");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Pulls a giant meteor from space, which deals 8 HP of damage to all affected units. Firepower and defense is increased.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Meteor Strike");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Prepare to embrace darkness!"),
                qsTr("You will tremble before my power!"),
                qsTr("Fear is all you have left..."),
                qsTr("You shall not survive!"),
                qsTr("Burning earth!!!"),
                qsTr("Such power... I regret crushing it.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Who would have thought you could oppose me?"),
                qsTr("My name is Sturm. Hear it and tremble."),
                qsTr("This is but a taste of my power!")];
    };
    this.getDefeatSentences = function(co)
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
