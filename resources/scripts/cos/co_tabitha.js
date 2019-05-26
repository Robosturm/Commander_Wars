var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(4);
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
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/tabitha.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_TABITHA.throwMeteor(co, 4, powerNameAnimation);
        audio.clearPlayList();
        CO_TABITHA.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_TABITHA.throwMeteor(co, 8, powerNameAnimation);
        audio.clearPlayList();
        CO_TABITHA.loadCOMusic(co);
        audio.playRandom();
    };

    this.throwMeteor = function(co, damage, powerNameAnimation)
    {
        // let a meteor fall :D
        var meteorTarget = co.getOwner().getRockettarget(2, damage);
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
        return 0;
    };
    this.getCOArmy = function()
    {
        return "DM";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
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
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
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
        return 10;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Belongs to the private army of Dark Matter. Eldest daughter of Caulder. Extremly cruel.");
    };
    this.getHits = function()
    {
        return qsTr("Breakfast in bed");
    };
    this.getMiss = function()
    {
        return qsTr("Stupid weak people");
    };
    this.getCODescription = function()
    {
        return qsTr("Her co unit is very weak outside of that she gets no bonuses.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("A small attack from the great owl that deals 4 HP of damage to all affected units. Also increases the firepower of all units.");
    };
    this.getPowerName = function()
    {
        return qsTr("Firestorm");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("A giant attack from the great owl that deals 8 HP of damage to all affected units. Firepower and defense is increased.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Apocalypse");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Input enemy's coordinates... And now let's enjoy the show!"),
                qsTr("Isn't this so much fun?"),
                qsTr("I hope you don't think i'm picking on you."),
                qsTr("Can you really be this weak? At least try to put up a fight.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("That was hardly worth the trouble."),
                qsTr("At least put up a fight!"),
                qsTr("I pity the weak!")];
    };
    this.getDefeatSentences = function()
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
