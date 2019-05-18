var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(5);
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
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_MINA.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_MINA.minaDamage(co, 4, powerNameAnimation);

        audio.clearPlayList();
        CO_MINA.loadCOMusic(co);
        audio.playRandom();
    };

    this.minaDamage = function(co, value, animation2)
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
                units.sortExpensive();
                var count = units.size() * 1 / 5;
                for (i = 0; i < count; i++)
                {
                    var unit = units.at(i);

                    animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                    if (animations.length < 5)
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                        if (animation2 !== null)
                        {
                            animation2.queueAnimation(animation);
                        }
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
                audio.addMusic("resources/music/cos/power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/mina.mp3")
                break;
        }
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 10;
                }
                return 0;
        }
    };

    this.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, luckMode)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
            return 0;
        case GameEnums.PowerMode_Power:
            if (luckMode === GameEnums.LuckDamageMode_On)
            {
                return globals.randInt(0, 50);
            }
            else if (luckMode === GameEnums.LuckDamageMode_Average)
            {
                return 25;
            }
            return 0;
        default:
            break;
        }
        return 0;
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "PF";
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A young enigmatic girl who serves Pinc Frontier. Deceptively dangerous.");
    };
    this.getHits = function()
    {
        return qsTr("Victory");
    };
    this.getMiss = function()
    {
        return qsTr("Uncertainty");
    };
    this.getCODescription = function()
    {
        return qsTr("Mina's troops have no real strength or weakness.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Unit's defenses may unexpectedly rise.");
    };
    this.getPowerName = function()
    {
        return qsTr("Potent of Misfortune");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Most expensive enemy units suffer five HP of damage.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Dark Lightning");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Enemy is in range. Activating counter-measures..."),
                qsTr("Please stop fighting! I don't want to do this!"),
                qsTr("Stop! ... ... Ignore that outburst, activate the weapons."),
                qsTr("The target will be eliminated shortly."),
                qsTr("A battle with me is a greater honor than you deserve!"),
                qsTr("Your defeat is now certain."),
                qsTr("I'm sorry...")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("No... What... ... ... The battle is over."),
                qsTr("What have I done?"),
                qsTr("The enemy has been routed. It is done.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("I hate loosing like this."),
                qsTr("This result ist wrong.")];
    };
    this.getName = function()
    {
        return qsTr("Mina");
    };
}

Constructor.prototype = CO;
var CO_MINA = new Constructor();
