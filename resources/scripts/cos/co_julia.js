var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(5);
        co.setSuperpowerStars(4);
    };

    this.activatePower = function(co)
    {

        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_JULIA.juliaStun(co, 0.5, powerNameAnimation);

        audio.clearPlayList();
        CO_JULIA.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_JULIA.juliaStun(co, 1, powerNameAnimation);

        audio.clearPlayList();
        CO_JULIA.loadCOMusic(co);
        audio.playRandom();
    };

    this.juliaStun = function(co, amount, animation2)
    {
        var player = co.getOwner();
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
                var count = amount * units.size()
                for (i = 0; i < count; i++)
                {
                    var unit = units.at(i);
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
                    animation.setEndOfAnimationCall("CO_JULIA", "postAnimationStun");
                }
                units.remove();
            }
        }
    };

    this.postAnimationStun = function(postAnimation)
    {
        postAnimation.seekBuffer();
        var x = postAnimation.readDataInt32();
        var y = postAnimation.readDataInt32();
        if (map.onMap(x, y))
        {
            var unit = map.getTerrain(x, y).getUnit();
            if (unit !== null)
            {
                unit.setHasMoved(true);
            }
        }
    };

    this.loadCOMusic = function(co)
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
                audio.addMusic("resources/music/cos/julia.mp3", 3969, 83932);
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "DM";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var baseDamage = 70;
        var fixedDamage = false;
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                baseDamage = 80;
                fixedDamage = true;
                break;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    baseDamage = 80;
                    fixedDamage = false;
                }
                break;
        }
        if (fixedDamage)
        {
            return baseDamage * 10 / attacker.getHpRounded() - 100;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A devoted CO who embraces Dark Matter's ideals and wishes to unite the world. She excels at utilizing propaganda to demoralize her enemies.");
    };
    this.getHits = function()
    {
        return qsTr("The morning paper");
    };
    this.getMiss = function()
    {
        return qsTr("The comics section");
    };
    this.getCODescription = function()
    {
        return qsTr("Units have reduced offensive power. However, firepower is unaffected by loss of HP.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Half of enemy units can't move next turn.");
    };
    this.getPowerName = function()
    {
        return qsTr("Rallying Cry");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("All enemy units can't move next turn.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Media Mayhem");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Your defeat is tomorrow's headline."),
                qsTr("Start the presses!"),
                qsTr("I don't receive the news. I make the news."),
                qsTr("I am the only voice of the people."),
                qsTr("Let us see how devoted your people are to your cause."),
                qsTr("You will come to embrace our ideals.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("The proof of our might will forever be etched in your minds."),
                qsTr("How foolish it was of you to try to defy us."),
                qsTr("This victory will be pleasing news to our citizens.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("The news will still call this a victory."),
                qsTr("I will change the news...")];
    };
    this.getName = function()
    {
        return qsTr("Julia");
    };
}

Constructor.prototype = CO;
var CO_JULIA = new Constructor();
