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
                audio.addMusic("resources/music/cos/bh_power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/kindle.mp3")
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
                animation.addSprite("power5", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
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

        CO_KINDLE.kindleDamage(co, 3, animations, powerNameAnimation);

        audio.clearPlayList();
        CO_KINDLE.loadCOMusic(co);
        audio.playRandom();
    };

    this.kindleDamage = function(co, value, animations)
    {
        var player = co.getPlayer();
        var units = player.getUnits();
        var counter = 0;
        var unit = null;
        var animation = null;
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
                    if (unit.getTerrain().getBuilding() !== null)
                    {
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
                            powerNameAnimation.queueAnimation(animation);
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
                }
                units.remove();
            }
        }
    };

    this.activateSuperpower = function(co)
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
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
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
        CO_KINDLE.loadCOMusic(co);
        audio.playRandom();
    };

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
        var building = map.getTerrain(atkPosX, atkPosY).getBuilding();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                var ret = 0;
                if (building !== null)
                {
                    ret = 130;
                }
                ret += co.getPlayer().getBuildingCount() * 3;
                return ret;
            case GameEnums.PowerMode_Power:
                if (building !== null)
                {
                    return 80;
                }
                break;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    if (building !== null)
                    {
                        return 40;
                    }
                }
                break;
        }
        if (building !== null)
        {
            return 20;
        }
        return 0;
    };


    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Jugger and Koal's commanding officer. Has a blunt, queen-like personality.");
    };
    this.getHits = function()
    {
        return qsTr("Anything chic");
    };
    this.getMiss = function()
    {
        return qsTr("Anything passe");
    };
    this.getCODescription = function()
    {
        return qsTr("An upper-crust CO who excels at urban warfare. Firepower of all units increased on properties.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Inflicts three HP of damage to enemy units on properties. Also increases firepower of all units on a property.");
    };
    this.getPowerName = function()
    {
        return qsTr("Urban Blight");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("The more properties she controls, the more firepower she gains. Also greatly increases firepower of all units on a property.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("High Society");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("So, you want to see me get serious? Very well! You asked for it!"),
                qsTr("Even the prettiest rose has thorns!"),
                qsTr("I am Kindle, Gorgeous AND Clever!"),
                qsTr("Let us play a more dangerous game, shall we? I grow weary of this."),
                qsTr("Aha ha ha! Beg for mercy, rat!"),
                qsTr("Still putting up a fight? Unbelievable!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Winning is almost as wonderful as I am. Aha ha ha!"),
                qsTr("I won again? This is getting so boring."),
                qsTr("Aha ha ha! Pathetic.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Losing to you corn-fed country folk is enough to give me frown lines."),
                qsTr("That was a bit of a disappointment.")];
    };
    this.getName = function()
    {
        return qsTr("Kindle");
    };
}

Constructor.prototype = CO;
var CO_KINDLE = new Constructor();
