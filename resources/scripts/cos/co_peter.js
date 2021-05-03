var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(3);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
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
            audio.addMusic("resources/music/cos/peter.mp3", 45424, 90079);
            break;
        }
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
            if (globals.randInt(0, 1) === 0)
            {
                animation.setSound("power8_1.wav");
            }
            else
            {
                animation.setSound("power8_2.wav");
            }
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
    };

    this.peterDamage = function(co, value, animation2)
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
                    if (map.getTerrain(unit.getX(), unit.getY()).getBuilding() === null)
                    {
                        animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                        animation.setSound("power4.wav");
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
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_PETER.peterDamage(co, 2, powerNameAnimation);
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
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
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (defender.getBaseMaxRange() === 1)
            {
                bonus += 60;
            }
            bonus += 10;
            break;
        case GameEnums.PowerMode_Power:
            if (defender.getBaseMaxRange() === 1)
            {
                bonus += 50;
            }
            bonus += 10;
            break;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (defender.getBaseMaxRange() === 1)
                {
                    bonus = 50;
                }
                bonus += 10;
            }
            else
            {
                if (defender.getBaseMaxRange() === 1)
                {
                    bonus = 7;
                }
            }
            break;
        }
        bonus -= defender.getTerrainDefense() * 5;
        return bonus;
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
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    this.getCOUnits = function(co, building)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
                buildingId === "TOWN" ||
                buildingId === "HQ")
        {
            return ["ZCOUNIT_ROYAL_GUARD"];
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A veteran CO with over 30 years of experience, mostly from the plains of Cosmo Land. Cautious but decisive.");
    };
    this.getHits = function(co)
    {
        return qsTr("Electric razors");
    };
    this.getMiss = function(co)
    {
        return qsTr("Excuses");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Peter's units plow through enemy direct-combat units with ease. However, thicker terrain weakens his attacks.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nRoyal Guard\n") +
                qsTr("\nGlobal Effect: \nDirect Units have increased firepower and loose additional firepower per terrain star.") +
                qsTr("\n\nCO Zone Effect: \nDirect Units have increased firepower.");
        return text;
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Firepower is greatly increased against enemy direct-combat units.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("General Offensive");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Enemy units not on a property suffer 2 HP of damage.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Bombing Run");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("We're going in, boys. Wait for the signal, then strike!"),
                qsTr("You won't even have time to take cover."),
                qsTr("It's time to tear through their tanks."),
                qsTr("Bogged down? Me? Ha! You seriously overestimate yourself."),
                qsTr("I've been down there with the grunts... I know what's feasible and what's not."),
                qsTr("I may not be unbeatable, but you're obviously not either.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Experience is the key to my success."),
                qsTr("I hope you learned something from this."),
                qsTr("What did you think you were doing with those tanks?")];
    };
    this.getDefeatSentences = function(co)
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
