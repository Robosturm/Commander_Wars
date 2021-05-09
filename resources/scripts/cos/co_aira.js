var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAlways(co, powerSurplus, turnMode);
    };

    this.activatePower = function(co)
    {

        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

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
                    if (globals.randInt(0, 1) === 0)
                    {
                        animation.setSound("power10_1.wav");
                    }
                    else
                    {
                        animation.setSound("power10_2.wav");
                    }
                    if (animations.length < 5)
                    {
                        animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                        if (powerNameAnimation !== null)
                        {
                            powerNameAnimation.queueAnimation(animation);
                        }
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_AIRA.airaDamage(co, 3, powerNameAnimation);
    };

    this.airaDamage = function(co, value, animation2)
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
                var size = units.size();
                for (i = 0; i < size; i++)
                {
                    var unit = units.at(i);
                    if (i >= size / 2 || unit.useTerrainDefense() === false)
                    {
                        animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                        animation.setSound("power4.wav");
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
                audio.addMusic("resources/music/cos/aira.mp3", 75, 58517);
                break;
        }
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
    this.coZoneBonus = 20;
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_AIRA.coZoneBonus;
            case GameEnums.PowerMode_Power:
                return CO_AIRA.coZoneBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_AIRA.coZoneBonus;
                }
                break;
        }
        return 0;
    };

    this.getMovementFuelCostModifier = function(co, unit, fuelCost)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power &&
            co.getOwner().isEnemyUnit(unit) === true)
        {
            return fuelCost * 5;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender)
        {
            if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_AIRA.coZoneBonus;
            }
            return 0;
        };

    this.getMovementcostModifier = function(co, unit, posX, posY)
    {
        if ((co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower) &&
            co.getOwner().isEnemyUnit(unit) === true)
        {
            return 1;
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A reserved commander who is often found daydreaming. Likes to walk in the wind.");
    };
    this.getHits = function(co)
    {
        return qsTr("Cool Breezes");
    };
    this.getMiss = function(co)
    {
        return qsTr("Surprises");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Aira never felt a need to specialize, so all units are average.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nGlobal Effect: \nNo bonus.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain %0% firepower and defence.");
        text = replaceTextArgs(text, [CO_ADDER.coZoneBonus]);
       return text;
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Enemies expend 6 times more fuel for the next day.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Gust Storm");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("The cheapest enemy units and air units suffer 3 HP of damage. All enemy units have increased movement costs.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Hurricannon");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Amazing what a little gust can do..."),
                qsTr("How can one fight against what they can't see or damage?"),
                qsTr("This is the only surprise I don't mind..."),
                qsTr("The winds will move the tides of war."),
                qsTr("A victory depends on whether my enemy can survive this weather."),
                qsTr("I am certain that my determination is not what one would call sub par.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("...Huh? Oh, right. I won. Woohoo?"),
                qsTr("I'm glad I expected this. A surprise would have been unbearable."),
                qsTr("Victory blew in my direction today.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("The wind blew in the wrong direction."),
                qsTr("Hmm! Oh I have dreamed too much.")];
    };
    this.getName = function()
    {
        return qsTr("Aira");
    };
}

Constructor.prototype = CO;
var CO_AIRA = new Constructor();
