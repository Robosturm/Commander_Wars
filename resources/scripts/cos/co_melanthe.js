var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
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
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/melanthe.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_MELANTHE.melantheDamage(co, 3, 0, powerNameAnimation);
        audio.clearPlayList();
        CO_MELANTHE.loadCOMusic(co);
        audio.playRandom();
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
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_MELANTHE.melantheDamage(co, 4, 2, powerNameAnimation);
        audio.clearPlayList();
        CO_MELANTHE.loadCOMusic(co);
        audio.playRandom();
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
                case GameEnums.PowerMode_Superpower:
                    return 2;
                case GameEnums.PowerMode_Power:
                    return 2;
                default:
                    return 1;
            }
        }
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
        if (map.onMap(atkPosX, atkPosY))
        {
            if (CO_MELANTHE.isNature(atkPosX, atkPosY) === true)
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Superpower:
                    return 20;
                case GameEnums.PowerMode_Power:
                    return 20;
                default:
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return 20;
                    }
                    break;
                }
                return 5;
            }
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A genius scientist that harbors a great hatred for humankind. Believes plants to be superior life forms.");
    };
    this.getHits = function()
    {
        return qsTr("Botany");
    };
    this.getMiss = function()
    {
        return qsTr("Bovines");
    };
    this.getCODescription = function()
    {
        return qsTr("Effects of natural terrain are increased by one star and firepower is increased on natural terrain");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Units on natural terrain restore three HP of health. Effects of natural terrain are increased by one star.");
    };
    this.getPowerName = function()
    {
        return qsTr("Synthetic Seedlings");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Enemies on natural terrain suffer two HP of damage. In addition, allied units on natural terrain restore four HP of health. Effects of natural terrain are increased by one star.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Agent Dreadroot");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Biological warfare is overrated; botanical warfare is supreme."),
				qsTr("The power of nature is on my side!"),
                qsTr("'Blades of grass' has a new meaning now, doesn't it?"),
				qsTr("Don't breathe in... heh heh heh."),
				qsTr("Ground up the enemy into fertilizer!"),
				qsTr("Don't have allergies? You will now.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("The seeds of success have been sown."),
				qsTr("The enemy was a lawn, and I its mower."),
				qsTr("Now that... that was fun.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("What Humans have defeated the almighty Nature."),
				qsTr("Nobody destroys the Nature. Next time...")];
    };
    this.getName = function()
    {
        return qsTr("Melanthe");
    };
}

Constructor.prototype = CO;
var CO_MELANTHE = new Constructor();
