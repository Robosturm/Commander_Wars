var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.getVehicleUnitIDS = function()
    {
        return ["FLAK", "FLARE", "HEAVY_HOVERCRAFT", "HEAVY_TANK",
                "HOVERCRAFT", "HOVERFLAK", "LIGHT_TANK", "MEGATANK",
                "NEOTANK", "RECON", "ANTITANKCANNON", "ARTILLERY", "MISSILE",
                "PIPERUNNER", "ROCKETTHROWER"];
    };
    this.getGroundIndirectUnitIDS = function()
    {
        return ["ANTITANKCANNON", "ARTILLERY", "MISSILE",
                "PIPERUNNER", "ROCKETTHROWER"];
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
                audio.addMusic("resources/music/cos/jake.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        var vehicleUnits = CO_JAKE.getVehicleUnitIDS();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_JAKE.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        var vehicleUnits = CO_JAKE.getVehicleUnitIDS();
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
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
        units.remove();

        audio.clearPlayList();
        CO_JAKE.loadCOMusic(co);
        audio.playRandom();
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        if (map.onMap(atkPosX, atkPosY))
        {
            var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Superpower:
                if (terrainID === "PLAINS")
                {
                    return 40;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (terrainID === "PLAINS")
                {
                    return 20;
                }
                break;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (terrainID === "PLAINS")
                    {
                        return 20;
                    }
                }
                break;
            }
            if (terrainID === "PLAINS")
            {
                return 10;
            }
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY)
    {
        var indirectUnits = CO_JAKE.getGroundIndirectUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (indirectUnits.indexOf(unit.getUnitID()) >= 0)
                {
                    return 1;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (indirectUnits.indexOf(unit.getUnitID()) >= 0)
                {
                    return 1;
                }
                break;
            default:
                    return 0;
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        var vehicleUnits = CO_JAKE.getVehicleUnitIDS();
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
        {
            if (vehicleUnits.indexOf(unit.getUnitID()) >= 0)
            {
                return 2;
            }
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A young, energetic Orange Star CO who is a top-notch tank commander.");
    };
    this.getHits = function()
    {
        return qsTr("Clubbin'");
    };
    this.getMiss = function()
    {
        return qsTr("Easy listening");
    };
    this.getCODescription = function()
    {
        return qsTr("Fights well in the open. Firepower of all units increased on plains.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Firepower is increased on plains. Firing range of indirect-combat units is increased by one.");
    };
    this.getPowerName = function()
    {
        return qsTr("Beat Down");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Firepower is greatly increased on plains. Firing range of indirect units is increased by one, and movement of direct units by two");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Block Rock");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Give it up, fool!"),
                qsTr("Prepare to get served."),
                qsTr("Prepare for a subpoena of pain! Yeah, that's lawyer style!"),
                qsTr("This is how I roll!"),
                qsTr("Wassup now?!"),
                qsTr("Here...have a taste!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Get the plates, 'cause you just got served!"),
                qsTr("Owned!"),
                qsTr("You got dropped like a phat beat!")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("I got spanked out there! This combat is the real deal..."),
                qsTr("Dude, we so don't have time for this.")];
    };
    this.getName = function()
    {
        return qsTr("Jake");
    };
}

Constructor.prototype = CO;
var CO_JAKE = new Constructor();
