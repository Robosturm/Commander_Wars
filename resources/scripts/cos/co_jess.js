var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
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
                audio.addMusic("resources/music/cos/jess.mp3")
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
            unit.refill();

            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_JESS.loadCOMusic(co);
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
            unit.refill();

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
        CO_JESS.loadCOMusic(co);
        audio.playRandom();
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "GE";
    };

    this.getAirUnitIDS = function()
    {
        return ["BOMBER", "FIGHTER", "DUSTER", "K_HELI", "T_HELI", "STEALTHBOMBER", "TRANSPORTPLANE", "WATERPLANE"];
    };
    this.getSeaUnitIDS = function()
    {
        return ["AIRCRAFTCARRIER", "CRUISER", "BATTLESHIP", "CANNONBOAT", "DESTROYER", "SUBMARINE"];
    };
    this.getInfatryUnitIDS = function()
    {
        return ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var airUnits = CO_JESS.getAirUnitIDS();
        var seaUnits = CO_JESS.getSeaUnitIDS();
        var infantryUnits = CO_JESS.getInfatryUnitIDS();

        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if ((airUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (seaUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (infantryUnits.indexOf(attacker.getUnitID()) < 0))
                {
                    return 70;
                }
                break;
            case GameEnums.PowerMode_Power:
                if ((airUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (seaUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (infantryUnits.indexOf(attacker.getUnitID()) < 0))
                {
                    return 50;
                }
                break;
            default:
                if ((airUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (seaUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (infantryUnits.indexOf(attacker.getUnitID()) < 0))
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return 30;
                    }
                    return 10;
                }
                break;
        }
        if ((airUnits.indexOf(attacker.getUnitID()) >= 0) ||
            (seaUnits.indexOf(attacker.getUnitID()) >= 0))
        {
            return -10;
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        var airUnits = CO_JESS.getAirUnitIDS();
        var seaUnits = CO_JESS.getSeaUnitIDS();
        var infantryUnits = CO_JESS.getInfatryUnitIDS();
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            if ((airUnits.indexOf(unit.getUnitID()) < 0) &&
                (seaUnits.indexOf(unit.getUnitID()) < 0) &&
                (infantryUnits.indexOf(unit.getUnitID()) < 0))
            {
                return 1;
            }
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
        {
            if ((airUnits.indexOf(unit.getUnitID()) < 0) &&
                (seaUnits.indexOf(unit.getUnitID()) < 0) &&
                (infantryUnits.indexOf(unit.getUnitID()) < 0))
            {
                return 2;
            }
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A gallant tank-driving commander who excels at analyzing information. Often argues with Eagle.");
    };
    this.getHits = function()
    {
        return qsTr("Dandelions");
    };
    this.getMiss = function()
    {
        return qsTr("Unfit COs");
    };
    this.getCODescription = function()
    {
        return qsTr("Vehicular units have superior firepower. Air and naval units are comparatively weak.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Movement range of vehicles increases by one space and their firepower increases. All units' fuel and ammunition supplies are replenished.");
    };
    this.getPowerName = function()
    {
        return qsTr("Turbo Charge");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Movement range of vehicles increases by two spaces and their firepower greatly increases. All units' fuel and ammunition supplies are replenished.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Overdrive");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Battle should never be taken lightly. I always give 100%! Always!!!"),
                qsTr("I won't let this chance pass me by!"),
                qsTr("Weakness has no place on the battlefield!"),
                qsTr("Here's my chance..."),
                qsTr("Time to turn the tables..."),
                qsTr("I've been waiting for this moment!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("This battle will advance my tactical research..."),
                qsTr("Looks like I survived another day."),
                qsTr("We made it... I'm glad that's done.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("This is looking grim..."),
                qsTr("It was inevitable under these conditions.")];
    };
    this.getName = function()
    {
        return qsTr("Jess");
    };
}

Constructor.prototype = CO;
var CO_JESS = new Constructor();
