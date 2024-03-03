var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.activatePower = function(co, map)
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
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power8_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power8_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_CASSIDY.cassidyDamage(co, CO_CASSIDY.superPowerDamage, dialogAnimation, map);
    };

    this.cassidyDamage = function(co, value, animation2, map)
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
                        animation2.queueAnimation(animation);
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
        }
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
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
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "TI";
    };

    this.superPowerDamage = 1;
    this.superPowerBonus = 80;

    this.powerBonus = 60;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;

    this.d2dOffBonus = 30;

    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneBonus = 50;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var getsBonus = false;
            if (defender !== null)
            {
                getsBonus = attacker.getHp() >= defender.getHp();
            }
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (getsBonus)
                {
                    return CO_CASSIDY.superPowerBonus;
                }
                return CO_CASSIDY.powerOffBonus;
            case GameEnums.PowerMode_Power:
                if (getsBonus)
                {
                    return CO_CASSIDY.powerBonus;
                }
                return CO_CASSIDY.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (getsBonus)
                    {
                        return CO_CASSIDY.d2dCoZoneBonus;
                    }
                    return CO_CASSIDY.d2dCoZoneOffBonus;
                }
                else if (getsBonus)
                {
                    if (map === null ||
                        (map !== null && map.getGameRules().getCoGlobalD2D()))
                    {
                        return CO_CASSIDY.d2dOffBonus;
                    }
                }
                break;
            }
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_CASSIDY.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_CASSIDY.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("An extremely belligerent member of Teal Island that is notorious for her ruthlessness. She fights purely for the thrill of battle and is feared by both enemies and allies.");
    };
    this.getHits = function(co)
    {
        return qsTr("Roses");
    };
    this.getMiss = function(co)
    {
        return qsTr("Violets");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Cassidy's ruthlessness rubs off on her troops. Units show no mercy to enemy units equal to or below their own strength.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_CASSIDY.d2dOffBonus];
        }

        var text = qsTr("\nGlobal Effect: \nCassidy's units gain +%0% firepower when attacking units with equal or less health.") +
                qsTr("\n\nCO Zone Effect: \nCassidy's units gain +%1% firepower when attacking units with equal or less health and +%2% firepower otherwise. Her units gain +%3% defence.");
        text = replaceTextArgs(text, [values[0], CO_CASSIDY.d2dCoZoneBonus, CO_CASSIDY.d2dCoZoneOffBonus, CO_CASSIDY.d2dCoZoneDefBonus]);
        return text;

    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Cassidy's units gain +%0% firepower when attacking units with equal or less health and +%1% firepower otherwise. Her units gain +%2% defence.");
        text = replaceTextArgs(text, [CO_CASSIDY.powerBonus, CO_CASSIDY.powerOffBonus, CO_CASSIDY.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Rampage");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("All enemy units suffer -%0 HP of damage. Cassidy's units gain +%1% firepower when attacking units with equal or less health and +%2% firepower otherwise. Her units gain +%3% defence.");
        text = replaceTextArgs(text, [CO_CASSIDY.superPowerDamage, CO_CASSIDY.superPowerBonus, CO_CASSIDY.powerOffBonus, CO_CASSIDY.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Scorched Earth");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Take no prisoners!"),
                qsTr("Ahahaha! I'll destroy you all!"),
                qsTr("You won't like me when I am angry!"),
                qsTr("I swear to make you rue this day!"),
                qsTr("Burn everything! I don't want to see anything left untouched!"),
                qsTr("I have fury!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("You better hope we do not meet again."),
                qsTr("I love this job!"),
                qsTr("I wish the battle had lasted a little longer. Oh well.")];
    };
    this.getDefeatSentences = function(co)
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
