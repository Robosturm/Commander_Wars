var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
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
        CO_CASSIDY.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_CASSIDY.cassidyDamage(co, 1, dialogAnimation);

        audio.clearPlayList();
        CO_CASSIDY.loadCOMusic(co);
        audio.playRandom();
    };

    this.cassidyDamage = function(co, value, animation2)
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
                for (i = 0; i < units.size(); i++)
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
                    animation.writeDataInt32(value);
                    animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
                }
                units.remove();
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
                audio.addMusic("resources/music/cos/cassidy.mp3", 76, 75279)
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "TI";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        if (defender !== null)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getHp() >= defender.getHp())
                {
                    return 80;
                }
                return 30;
            case GameEnums.PowerMode_Power:
                if (attacker.getHp() >= defender.getHp())
                {
                    return 60;
                }
                return 30;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getHp() >= defender.getHp())
                    {
                        return 50;
                    }
                    return 10;
                }
                break;
            }
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
        return qsTr("An extremely belligerent member of Teal Island that is notorious for her ruthlessness. She fights purely for the thrill of battle and is feared by both enemies and allies.");
    };
    this.getHits = function()
    {
        return qsTr("Roses");
    };
    this.getMiss = function()
    {
        return qsTr("Violets");
    };
    this.getCODescription = function()
    {
        return qsTr("Cassidy's units show no mercy. Increased firepower when attacking a unit with equal or less health.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nNo bonus.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain additional 50% firepower when attacking units with equal or less health.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Firepower is increased even further when attacking a unit with equal or less health.");
    };
    this.getPowerName = function()
    {
        return qsTr("Rampage");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("All enemies suffer one HP of damage. Firepower is greatly increased when attacking a unit with equal or less health.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Scorched Earth");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Take no prisoners!"),
                qsTr("Ahahaha… I'll destroy you all!"),
                qsTr("You won't like me when I am angry!"),
                qsTr("I swear to make you rue this day!"),
                qsTr("Burn everything! I don't want to see anything left untouched!"),
                qsTr("I have fury!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("You better hope we do not meet again."),
                qsTr("I love this job!"),
                qsTr("I wished the battle had lasted a little longer. Oh well.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("You will regret this..."),
                qsTr("Kill them... kill them all...")];
    };
    this.getName = function()
    {
        return qsTr("Cassidy");
    };
}

Constructor.prototype = CO;
var CO_CASSIDY = new Constructor();
