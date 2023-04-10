var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(4);
    };

    this.loadCOMusic = function(co, map)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0 , 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0 , 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/tabitha.mp3", 56155, 127410);
                break;
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_TABITHA.throwMeteor(co, 4, powerNameAnimation, map);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_TABITHA.throwMeteor(co, 8, powerNameAnimation, map);
    };
    this.throwMeteor = function(co, damage, powerNameAnimation, map)
    {
        // let a meteor fall :D
        var rocketTarget = co.getOwner().getRockettarget(2, damage);
        // create cool meteor animation :)
        var animation = GameAnimationFactory.createAnimation(map, rocketTarget.x - 2, rocketTarget.y - 2 - 1);
        animation.addSprite("explosion+silo", -map.getImageSize() / 2, 0, 0, 2, 0);
        animation.setSound("missle_explosion.wav", 1);
        animation.setStartOfAnimationCall("CO_TABITHA", "preAnimationThrowMeteor");
        animation.setEndOfAnimationCall("CO_TABITHA", "postAnimationThrowMeteor");        
        powerNameAnimation.queueAnimation(animation);
        CO_TABITHA.postAnimationThrowMeteorTarget = rocketTarget;
        CO_TABITHA.postAnimationThrowMeteorDamage = damage;
    };

    this.preAnimationThrowMeteor = function(animation, map)
    {
        map.centerMap(CO_TABITHA.postAnimationThrowMeteorTarget.x, CO_TABITHA.postAnimationThrowMeteorTarget.y);
    };
    this.postAnimationThrowMeteorTarget = null;
    this.postAnimationThrowMeteorDamage = 0;
    this.postAnimationThrowMeteor = function(animation, map)
    {
        var meteorTarget = CO_TABITHA.postAnimationThrowMeteorTarget;
        var damage = CO_TABITHA.postAnimationThrowMeteorDamage;
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
        CO_TABITHA.postAnimationThrowMeteorTarget = null;
        CO_TABITHA.postAnimationThrowMeteorDamage = 0;
    };

    this.getCOUnitRange = function(co, map)
    {
        return 0;
    };
    this.getCOArmy = function()
    {
        return "DM";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 60;
            case GameEnums.PowerMode_Power:
                return 40;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 60;
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
                return 60;
            case GameEnums.PowerMode_Power:
                return 40;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return 60;
                }
                break;
        }
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Belongs to the private army of Dark Matter. Eldest daughter of Caulder. Extremely cruel.");
    };
    this.getHits = function(co)
    {
        return qsTr("Breakfast in bed");
    };
    this.getMiss = function(co)
    {
        return qsTr("Stupid weak people");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Her co unit is strong, weak outside of that she gets no bonuses.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nNo Effects") +
               qsTr("\n\nCO Zone Effect: \nUnits have 60% increased firepower and defense.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("A small attack from the great owl that deals 4 HP of damage to all affected units. Also increases the firepower of all units.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Firestorm");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("A giant attack from the great owl that deals 8 HP of damage to all affected units. Firepower and defense is increased.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Apocalypse");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Input enemy's coordinates... And now let's enjoy the show!"),
                qsTr("Isn't this so much fun?"),
                qsTr("I hope you don't think i'm picking on you."),
                qsTr("Can you really be this weak? At least try to put up a fight.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("That was hardly worth the trouble."),
                qsTr("At least put up a fight!"),
                qsTr("I pity the weak!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I'm not stronger?"),
                qsTr("I had no breakfast that's the problem.")];
    };
    this.getName = function()
    {
        return qsTr("Tabitha");
    };
}

Constructor.prototype = CO;
var CO_TABITHA = new Constructor();
