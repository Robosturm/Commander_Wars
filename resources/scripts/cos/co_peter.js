var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(2);
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
                audio.addMusic("resources/music/cos/peter.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
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
                animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_PETER.loadCOMusic(co);
        audio.playRandom();
    };

    this.peterDamage = function(co, value, animation2)
    {
        var player = co.getPlayer();
        var counter = 0;
        var playerCounter = map.getPlayerCount();
        var animation = null;
        var animations = [];

        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {

                var units = enemyPlayer.getUnits();
                units.randomize();
                for (i = 0; i < units.size(); i++)
                {
                    var unit = units.at(i);
                    if (map.getTerrain(unit.getX(), unit.getY()).getBuilding() === null)
                    {
                        animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                        if (animations.length < 5)
                        {
                            animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                            animation2.queueAnimation(animation);
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
                        animation.writeDataInt32(unit.getX());
                        animation.writeDataInt32(unit.getY());
                        animation.writeDataInt32(value);
                        animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
                    }
                }
                units.remove();
            }
        }
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_PETER.peterDamage(co, 2, powerNameAnimation);
        audio.clearPlayList();
        CO_PETER.loadCOMusic(co);
        audio.playRandom();
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "GE";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        if (defender === null)
        {
            return 0;
        }
        var bonus = 0;
        if (defender.getMaxRange() === 1)
        {

            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Superpower:
                    return 40;
                case GameEnums.PowerMode_Power:
                    return 60;
                default:
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        bonus = 40;
                    }
                    else
                    {
                        bonus = 20;
                    }
                    break;
            }
        }
        bonus -= defender.getTerrainDefense() * 5;
        return bonus;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A veteran CO with over 30 years of experience, mostly from the plains of Cosmo Land. Cautious but decisive.");
    };
    this.getHits = function()
    {
        return qsTr("Electric razors");
    };
    this.getMiss = function()
    {
        return qsTr("Excuses");
    };
    this.getCODescription = function()
    {
        return qsTr("Peter's units plow through enemy direct-combat units with ease. However, thicker terrain weakens his attacks.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Firepower is greatly increases against enemy direct-combat units.");
    };
    this.getPowerName = function()
    {
        return qsTr("General Offensive");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Enemy units not on a property suffer 2 HP of damage.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Bombing Run");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("We're going in, boys. Wait for the signal, then strike!"),
				qsTr("You won't even have time to take cover."),
				qsTr("It's time to tear through their tanks."),
				qsTr("Bogged down? Me? Ha! You seriously overestimate yourself."),
				qsTr("I've been down there with the grunts... I know what's feasible and what's not."),
				qsTr("I may not be unbeatable, but you're obviously not either.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Experience is the key to my success."),
				qsTr("I hope you learned something from this."),
				qsTr("What did you think you were doing with those tanks?")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Your tanks were better than mine."),
				qsTr("You're never to old to learn some new moves.")];
    };
    this.getName = function()
    {
        return qsTr("Peter");
    };
}

Constructor.prototype = CO;
var CO_PETER = new Constructor();
