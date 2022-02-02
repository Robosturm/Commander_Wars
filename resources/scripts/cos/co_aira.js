var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
        return ["+alt"];
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAlways(co, powerSurplus, turnMode);
    };

    this.activatePower = function(co, map)
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
                    animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                    var delay = globals.randInt(135, 265);
                    if (animations.length < 5)
                    {
                        delay *= i;
                    }
                    if (i % 2 === 0)
                    {
                        animation.setSound("power10_1.wav", 1, delay);
                    }
                    else
                    {
                        animation.setSound("power10_2.wav", 1, delay);
                    }
                    if (animations.length < 5)
                    {
                        animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        if (powerNameAnimation !== null)
                        {
                            powerNameAnimation.queueAnimation(animation);
                        }
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_AIRA.airaDamage(co, 3, powerNameAnimation, map);
    };

    this.airaDamage = function(co, value, animation2, map)
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
                        animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                        var delay = globals.randInt(135, 265);
                        if (animations.length < 5)
                        {
                            delay *= i;
                        }
                        animation.setSound("power4.wav", 1, delay);
                        if (animations.length < 5)
                        {
                            animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                            if (animation2 !== null)
                            {
                                animation2.queueAnimation(animation);
                            }
                            animations.push(animation);
                        }
                        else
                        {
                            animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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

    this.loadCOMusic = function(co, map)
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
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "PF";
    };
    this.coZoneBonus = 20;
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
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

    this.getMovementFuelCostModifier = function(co, unit, fuelCost, map)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power &&
            co.getOwner().isEnemyUnit(unit) === true)
        {
            return fuelCost * 5;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isAttacker, action, luckmode, map)
        {
            if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_AIRA.coZoneBonus;
            }
            return 0;
        };

    this.getMovementcostModifier = function(co, unit, posX, posY, map)
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
            return qsTr("Enemies expend 6 times more fuel when they move for their next turn.");
        };
    this.getPowerName = function(co)
    {
        return qsTr("Gust Storm");
    };
    this.getSuperPowerDescription = function(co)
        {
            return qsTr("The cheapest enemy ground and sea units suffer 3 HP of damage while all enemy air units suffer 3 HP of damage. Lastly, all enemy units have half of their movement range.");
        };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Hurricannon");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Amazing what a little gust can do..."),
                qsTr("You cannot hope to face the power of the wind."),
                qsTr("The only surpises I like are the ones that help me win."),
                qsTr("The winds will help me blow through this battle."),
                qsTr("I can stand tall with the flow of wind, I cannot say the same for you."),
                qsTr("I am certain that my determination will grant me the favor of the winds.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("...Huh? Oh, right. I won. Woohoo?"),
                qsTr("This win was expected. Losing would have been a surprise, and I hate both."),
                qsTr("You could say... you were swept under the wind.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("The wind blew in the wrong direction."),
                qsTr("Luckily for you it was not windy today.")];
    };
    this.getName = function()
    {
        return qsTr("Aira");
    };
}

Constructor.prototype = CO;
var CO_AIRA = new Constructor();
