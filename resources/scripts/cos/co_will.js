var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.activatePower = function(co)
    {

        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(false);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (unit.getMaxRange() === 1)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                if (animations.length < 5)
                {
                    animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
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
        }
        units.remove();

        audio.clearPlayList();
        CO_WILL.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(true);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (unit.getMaxRange() === 1)
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
        CO_WILL.loadCOMusic(co);
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
            default:
                audio.addMusic("resources/music/cos/will.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };

    this.getSeaAirUnitIDS = function()
    {
        return ["BOMBER", "FIGHTER", "DUSTER", "K_HELI", "T_HELI", "STEALTHBOMBER", "TRANSPORTPLANE", "WATERPLANE",
                "AIRCRAFTCARRIER", "CRUISER", "BATTLESHIP", "CANNONBOAT", "DESTROYER", "SUBMARINE", "LANDER",
                "BLACK_BOMB", "BLACK_BOAT", "APC"];
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var seaAirUnits = CO_LIN.getSeaAirUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (attacker.getMaxRange() === 1 && seaAirUnits.indexOf(attacker.getUnitID()) < 0)
                {
                    return 50;
                }
                return 0;
            case GameEnums.PowerMode_Power:
                if (attacker.getMaxRange() === 1 && seaAirUnits.indexOf(attacker.getUnitID()) < 0)
                {
                    return 20;
                }
                return 0;
            default:
                if (attacker.getMaxRange() === 1 && seaAirUnits.indexOf(attacker.getUnitID()) < 0)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                    {
                        return 20;
                    }
                    else
                    {
                        return 10;
                    }
                }
                break;
        }
        return 0;
    };    

    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        var seaAirUnits = CO_LIN.getSeaAirUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (seaAirUnits.indexOf(unit.getUnitID()) < 0)
                {
                    return 2;
                }
                return 0;
            case GameEnums.PowerMode_Power:
                if (seaAirUnits.indexOf(unit.getUnitID()) < 0)
                {
                    return 1;
                }
                return 0;
            default:
                return 0;
        }
    };
	
    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Former Brown Desert cadet. Joined Amber Corona after being rescued by Brenner.");
    };
    this.getHits = function()
    {
        return qsTr("Brenner, Isabella");
    };
    this.getMiss = function()
    {
        return qsTr("The Beast, Greayfield, Caulder");
    };
    this.getCODescription = function()
    {
        return qsTr("Her units are slightly stronger.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Boost movement by 1 and increases the firepower of direct ground units.");
    };
    this.getPowerName = function()
    {
        return qsTr("Begin of an Era");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Boost movement by 2 and firepower of direct ground units.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("New Era");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("This is our chance to win. Prepare to move out."),
                qsTr("The captain saved my life. I need to take care of it."),
                qsTr("I'll never give up..."),
                qsTr("There's no time. Now or never...")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("I did it!"),
                qsTr("I'll never give up..."),
                qsTr("Is everyone okay?")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Never give up hope."),
                qsTr("I can't remember anything")];
    };
    this.getName = function()
    {
        return qsTr("Will");
    };
}

Constructor.prototype = CO;
var CO_WILL = new Constructor();
