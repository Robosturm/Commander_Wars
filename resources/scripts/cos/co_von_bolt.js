var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(6);
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
            default:
                audio.addMusic("resources/music/cos/von_bolt.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(false);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_VON_BOLT.throwLaserray(co, 1, 2, powerNameAnimation);
        audio.clearPlayList();
        CO_VON_BOLT.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(true);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_VON_BOLT.throwLaserray(co, 3, 3, powerNameAnimation);
        audio.clearPlayList();
        CO_VON_BOLT.loadCOMusic(co);
        audio.playRandom();
    };

    this.throwLaserray = function(co, damage, range, powerNameAnimation)
    {
        // let a meteor fall :D
        var meteorTarget = co.getPlayer().getRockettarget(range, damage);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);

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
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 20;
                }
                break;
        }
        return 5;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 20;
            case GameEnums.PowerMode_Power:
                return 20;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return 20;
                }
                break;
        }
        return 5;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Former commander-in-chief of the Black Hole forces. A mysterious old man who has been alive a very, very long time. Masterminded of the Omega war. Immortal Evil");
    };
    this.getHits = function()
    {
        return qsTr("Long life");
    };
    this.getMiss = function()
    {
        return qsTr("Young 'uns!");
    };
    this.getCODescription = function()
    {
        return qsTr("All units have superior firepower and defense.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Fires shock waves that disables electical systems and all forces in range become paralyzed. Affected units suffer one HP of damage. Firepower and defense rises.");
    };
    this.getPowerName = function()
    {
        return qsTr("Shockwave");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Fires shock waves that disables electical systems and all forces in range become paralyzed. Affected units suffer three HP of damage. Firepower and defense rises.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Deus Ex Machina");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("No matter how powerful...hhh...you are, you're...hhh...still a worm on a hook..."),
                qsTr("Well...hhh...let me have a taste...hhh...of your soul."),
                qsTr("Hheh heh heh... Stop panicking... it's...hhh...almost over..."),
                qsTr("Hheh heh heh. The darkness rises..."),
                qsTr("The sweet scent of destruction...hhh...it makes me...hhh...feel young!"),
                qsTr("Hold still...hhh...I'm going to...hhhhh...suck the marrow from your bones.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Predator...prey...hhh... I hunt them all..."),
                qsTr("Hehh hhh hhh... Pathetic."),
                qsTr("Even death...hhh...fears me...")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("What? Why are you Jouth so strong?"),
                qsTr("Nooo...I want to live forever.")];
    };
    this.getName = function()
    {
        return qsTr("Von Bolt");
    };
}

Constructor.prototype = CO;
var CO_VON_BOLT = new Constructor();
