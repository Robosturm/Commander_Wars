var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(5);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
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
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_MINA.minaDamage(co, 4, powerNameAnimation);
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
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/mina.mp3", 76, 100073);
                break;
        }
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 20;
            case GameEnums.PowerMode_Power:
                return 20;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 20;
                }
                return 0;
        }
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 20;
        }
        return 0;
    };
    this.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, luckMode)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
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
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
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
    this.getBio = function(co)
    {
        return qsTr("A young enigmatic girl who serves Pink Frontier. Deceptively dangerous.");
    };
    this.getHits = function(co)
    {
        return qsTr("Victory");
    };
    this.getMiss = function(co)
    {
        return qsTr("Uncertainty");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Mina's troops have no real strength or weakness.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain additional firepower and defense.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Unit's defenses may unexpectedly rise.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Potent of Misfortune");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Most expensive enemy units suffer five HP of damage.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Dark Lightning");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Enemy is in range. Activating counter-measures..."),
                qsTr("Please stop fighting! I don't want to do this!"),
                qsTr("Stop! ... ... Ignore that outburst, activate the weapons."),
                qsTr("The target will be eliminated shortly."),
                qsTr("A battle with me is a greater honor than you deserve!"),
                qsTr("Your defeat is now certain."),
                qsTr("I'm sorry...")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("No... What... ... ... The battle is over."),
                qsTr("What have I done?"),
                qsTr("The enemy has been routed. It is done.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I hate losing like this."),
                qsTr("This result is wrong.")];
    };
    this.getName = function()
    {
        return qsTr("Mina");
    };
}

Constructor.prototype = CO;
var CO_MINA = new Constructor();
