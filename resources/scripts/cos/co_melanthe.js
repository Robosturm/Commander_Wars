var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
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
        return CO.getAiUsePowerAtUnitCount(co, powerSurplus, turnMode, repairUnits);
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
                audio.addMusic("resources/music/cos/melanthe.mp3", 2716, 75957);
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_MELANTHE.melantheDamage(co, 3, 0, powerNameAnimation);
    };

    this.melantheDamage = function(co, heal, damage, powerNameAnimation)
    {

        var player = co.getOwner();
        var units = player.getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (CO_MELANTHE.isNature(unit.getX(), unit.getY()) === true)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());

                animation.writeDataInt32(unit.getX());
                animation.writeDataInt32(unit.getY());
                animation.writeDataInt32(heal);
                animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
                animation.setSound("power3.wav");
                if (animations.length < 5)
                {
                    animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        if (damage > 0)
        {
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

                        if (CO_MELANTHE.isNature(unit.getX(), unit.getY()) === true)
                        {
                            animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                            animation.writeDataInt32(unit.getX());
                            animation.writeDataInt32(unit.getY());
                            animation.writeDataInt32(damage);
                            animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
                            animation.setSound("power4.wav");
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
        }
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_MELANTHE.melantheDamage(co, 4, 2, powerNameAnimation);
    };

    this.isNature = function(posX, posY)
    {
        var terrain = map.getTerrain(posX, posY);
        var building = terrain.getBuilding();
        if (building !== null)
        {
            return false;
        }
        if (terrain.getID() === "STREET" ||
            terrain.getID() === "BRIDGE" ||
            terrain.getID() === "RUIN" )
        {
            return false;
        }
        return true;
    };

    this.getTerrainDefenseModifier = function(co, unit, posX, posY)
    {
        if (CO_MELANTHE.isNature(posX, posY) === true)
        {
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    return 2;
                case GameEnums.PowerMode_Power:
                    return 2;
                default:
                    if (co.inCORange(Qt.point(posX, posY), unit))
                    {
                        return 2;
                    }
            }
        }
        return 0;
    };
    this.getRepairBonus = function(co, unit, posX, posY)
    {
        return -1;
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "DM";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        if (typeof map !== 'undefined')
        {
            if (map.onMap(atkPosX, atkPosY))
            {
                switch (co.getPowerMode())
                {
                    case GameEnums.PowerMode_Tagpower:
                    case GameEnums.PowerMode_Superpower:
                    case GameEnums.PowerMode_Power:
                        if (CO_MELANTHE.isNature(atkPosX, atkPosY) === true)
                        {
                            return 50;
                        }
                        return 10;
                    default:
                        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                        {
                            if (CO_MELANTHE.isNature(atkPosX, atkPosY) === true)
                            {
                                return 40;
                            }
                            return 10;
                        }
                }
                return 0;
            }
        }
        else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            return 10;
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
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A genius scientist that harbors a great hatred for humankind. Believes plants to be a superior life forms.");
    };
    this.getHits = function(co)
    {
        return qsTr("Botany");
    };
    this.getMiss = function(co)
    {
        return qsTr("Bovines");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Effects of natural terrain are increased by one star and firepower is increased on natural terrain. Units are repaired by 1 HP less on Buildings.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nNo bonus.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain an additional firepower on natural terrain and increased terrain defence stars.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Units on natural terrain restore three HP of health. Effects of natural terrain are increased.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Synthetic Seedlings");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Enemies on natural terrain suffer two HP of damage. In addition, allied units on natural terrain restore four HP of health. Effects of natural terrain are increased.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Agent Dreadroot");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Biological warfare is overrated; botanical warfare is supreme."),
				qsTr("The power of nature is on my side!"),
                qsTr("'Blades of grass' has a new meaning now, doesn't it?"),
				qsTr("Don't breathe in... heh heh heh."),
				qsTr("Ground up the enemy into fertilizer!"),
				qsTr("Don't have allergies? You will now.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("The seeds of success have been sown."),
				qsTr("The enemy was a lawn, and I its mower."),
				qsTr("Now that... that was fun.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("What? Humans have defeated the almighty Nature."),
				qsTr("Nobody destroys the Nature. Next time...")];
    };
    this.getName = function()
    {
        return qsTr("Melanthe");
    };
}

Constructor.prototype = CO;
var CO_MELANTHE = new Constructor();
