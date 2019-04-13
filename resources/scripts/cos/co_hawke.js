var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(5);
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
                audio.addMusic("resources/music/cos/hawke.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        CO_HAWKE.hawkeDamage(co, 1);
        audio.clearPlayList();
        CO_HAWKE.loadCOMusic(co);
        audio.playRandom();
    };

    this.hawkeDamage = function(co, value)
    {

        var player = co.getPlayer();
        var units = player.getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            unit.setHp(unit.getHpRounded() + value);

            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
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

    this.activateSuperpower = function(co)
    {
        CO_HAWKE.hawkeDamage(co, 2);
        audio.clearPlayList();
        CO_HAWKE.loadCOMusic(co);
        audio.playRandom();
    };

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

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A commander of the Black Hole army who willstop at nothing to achieve his goals.");
    };
    this.getHits = function()
    {
        return qsTr("Black Coffee");
    };
    this.getMiss = function()
    {
        return qsTr("Incompetence");
    };
    this.getCODescription = function()
    {
        return qsTr("All units possess superior firepower. However, his CO power gauge is longer than it should be.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("All enemy units suffer one HP of damage. In addition, all allied units recover one HP.");
    };
    this.getPowerName = function()
    {
        return qsTr("Black Wave");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("All enemy units suffer two HP of damage. In addition, all allied units recover two HP.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Black Storm");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("This is the end.|Farewell."),
                qsTr("I give you credit for pushing me this far."),
                qsTr("You have inspired me to take action."),
                qsTr("To be defeated is to lose everything. Are you prepared for that outcome?"),
                qsTr("You leave me no choice.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Will you sacrifice all? Then perhaps you can win."),
                qsTr("Sad...you were no match for me."),
                qsTr("There is no chance.  My victory was assured.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("That's it then. We shall withdraw."),
                qsTr("... This is one situation I never expected myself to be in.")];
    };
    this.getName = function()
    {
        return qsTr("Hawke");
    };
}

Constructor.prototype = CO;
var CO_HAWKE = new Constructor();
