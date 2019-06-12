var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.getDirectUnitIDS = function()
    {
        return ["BOMBER", "CANNONBOAT", "CRUISER", "DESTROYER", "DUSTER",
                "FIGHTER", "FLAK", "FLARE", "HEAVY_HOVERCRAFT", "HEAVY_TANK",
                "HOVERCRAFT", "HOVERFLAK", "K_HELI", "LIGHT_TANK", "MEGATANK",
                "NEOTANK", "RECON", "STEALTHBOMBER", "SUBMARINE", "WATERPLANE"];
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
                audio.addMusic("resources/music/cos/grit.mp3")
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
            if (unit.getBaseMaxRange() > 1)
            {
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
        }
        units.remove();

        audio.clearPlayList();
        CO_GRIT.loadCOMusic(co);
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
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (unit.getBaseMaxRange() > 1)
            {
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
        CO_GRIT.loadCOMusic(co);
        audio.playRandom();
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var tankUnits = CO_GRIT.getDirectUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getBaseMaxRange() > 1)
                {
                    return 70;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (attacker.getBaseMaxRange() > 1)
                {
                    return 50;
                }
                break;
            default:
                if (attacker.getBaseMaxRange() > 1)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return 20;
                    }
                    return 0;
                }
                break;
        }
        if (tankUnits.indexOf(attacker.getUnitID()) >= 0)
        {
            return -20;
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (unit.getBaseMaxRange() > 1)
                {
                    return 3;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (unit.getBaseMaxRange() > 1)
                {
                    return 2;
                }
                break;
            default:
                if (unit.getBaseMaxRange() > 1)
                {
                    return 1;
                }
                break;
        }
        return 0;
    };
    this.getCOArmy = function()
    {
        return "BM";
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A laid-back style masks his dependability. A peerless marksman. Works well with Olaf.");
    };
    this.getHits = function()
    {
        return qsTr("Cats");
    };
    this.getMiss = function()
    {
        return qsTr("Rats");
    };
    this.getCODescription = function()
    {
        return qsTr("Indirect-combat units cause more damage and have increased firerange. Weak in non-infantry direct combat.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Increases range of indirect units by one space. Firepower of these units also rise.");
    };
    this.getPowerName = function()
    {
        return qsTr("Snipe Attack");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Increases range of indirect units by two spaces. Firepower of these units greatly rise.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Super Snipe");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Once you're in my sights, there's no gettin' away!"),
                qsTr("Reckon it's time to take you down!"),
                qsTr("Where's the fool who wants to help me with target practice?"),
                qsTr("Y'all can run, but you can't hide!"),
                qsTr("Y'all gimme no choice... Time to bring in the big guns!"),
                qsTr("Aw, shucks. I was hopin' it wouldn't come to this.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("This ain't for show."),
                qsTr("Maybe now I can get some shut-eye."),
                qsTr("I hope this gets easier. That was harder'n college!")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Aw, possum spit!"),
                qsTr("Just as I reckoned... This ain't gonna be no Sunday stroll.")];
    };
    this.getName = function()
    {
        return qsTr("Grit");
    };
}

Constructor.prototype = CO;
var CO_GRIT = new Constructor();
