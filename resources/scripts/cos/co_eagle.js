var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(6);
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        var unitInfantryIDs = ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (unitInfantryIDs.indexOf(unit.getUnitID())  < 0)
            {
                unit.setHasMoved(false);
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());

                if (animations.length < 5)
                {
                    animation.addSprite("power1", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power1", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
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

        audio.clearPlayList();
        CO_EAGLE.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        var unitInfantryIDs = ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (unitInfantryIDs.indexOf(unit.getUnitID())  < 0)
            {
                unit.setHasMoved(false);
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());

                if (animations.length < 5)
                {
                    animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
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

        audio.clearPlayList();
        CO_EAGLE.loadCOMusic(co);
        audio.playRandom();
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
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/eagle.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "GE";
    };

    this.getAirUnitIDS = function()
    {
        return ["BOMBER", "FIGHTER", "BLACK_BOMB", "DUSTER", "K_HELI", "T_HELI", "STEALTHBOMBER", "TRANSPORTPLANE", "WATERPLANE"];
    };
    this.getSeaUnitIDS = function()
    {
        return ["AIRCRAFTCARRIER", "CRUISER", "BATTLESHIP", "CANNONBOAT", "BLACK_BOAT", "DESTROYER", "SUBMARINE"];
    };

    this.getFuelCostModifier = function(co, unit, costs)
    {
        var airUnits = CO_EAGLE.getAirUnitIDS();
        if (airUnits.indexOf(unit.getUnitID()) >= 0)
        {
            return -2;
        }
        return 0;
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var airUnits = CO_EAGLE.getAirUnitIDS();
        var seaUnits = CO_EAGLE.getSeaUnitIDS();

        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (airUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 30;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (airUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return -30;
                }
                else if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return -40;
                }
                else
                {
                    return -45;
                }
            default:
                if (airUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return 20;
                    }
                    return 10;
                }
                break;
        }
        if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
        {
            return -10;
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Green Earth's daring pilot hero. Joined the air force to honor his father's legacy.");
    };
    this.getHits = function()
    {
        return qsTr("Lucky goggles");
    };
    this.getMiss = function()
    {
        return qsTr("Swimming");
    };
    this.getCODescription = function()
    {
        return qsTr("Air units use less fuel and have superior firepower. Naval units have weaker firepower.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("All non-infantry units that have already carried out orders may move again, but their firepower is cut in half.");
    };
    this.getPowerName = function()
    {
        return qsTr("Lightning Drive");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("All non-infantry units that have already carried out orders may move again.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Lightning Strike");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Checkmate!"),
                qsTr("All units, ready for attack! Don't even give them time for regret!"),
                qsTr("Do you think you can keep up?"),
                qsTr("The Eagle soars above you!"),
                qsTr("I have no interest in underlings. Begone!"),
                qsTr("I've caught you with your guard down!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Hmph! As expected!"),
                qsTr("Where's the challenge in that?"),
                qsTr("Another victory! Was there ever any doubt?")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("This is what happens when you lose focus!"),
                qsTr("Impossible! I'm getting angrier by the moment!")];
    };
    this.getName = function()
    {
        return qsTr("Eagle");
    };
}

Constructor.prototype = CO;
var CO_EAGLE = new Constructor();
