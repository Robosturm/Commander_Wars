var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(2);
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
            if (animations.length < 5)
            {
                animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_LIN.loadCOMusic(co);
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
        CO_LIN.loadCOMusic(co);
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
                audio.addMusic("resources/music/cos/lin.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 1;
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
                if (seaAirUnits.indexOf(attacker.getUnitID()) < 0)
                {
                    return 50;
                }
                return 0;
            case GameEnums.PowerMode_Power:
                if (seaAirUnits.indexOf(attacker.getUnitID()) < 0)
                {
                    return 30;
                }
                return 0;
            default:
                if (seaAirUnits.indexOf(attacker.getUnitID()) < 0)
                {

                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return 30;
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

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var seaAirUnits = CO_LIN.getSeaAirUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (seaAirUnits.indexOf(defender.getUnitID()) < 0)
                {
                    return 50;
                }
                return 0;
            case GameEnums.PowerMode_Power:
                if (seaAirUnits.indexOf(defender.getUnitID()) < 0)
                {
                    return 30;
                }
                return 0;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    if (seaAirUnits.indexOf(defender.getUnitID()) < 0)
                    {
                        return 30;
                    }
                }
                break;
        }
        return 0;
    };

    this.getVisionrangeModifier = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 2;
            case GameEnums.PowerMode_Power:
                return 1;
            default:
                return 0;
        }
    };

    this.getPerfectVision = function(co)
    {
        // return true if forest and reafs etc are disabled in fog of war see sonja
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return true;
            case GameEnums.PowerMode_Power:
                return true;
            default:
                return false;
        }
    };
	
    // CO - Intel
    this.getBio = function()
    {
        return qsTr("In battle the clearest thinker wins. A calm commander of Amber Corona.");
    };
    this.getHits = function()
    {
        return qsTr("Teasing Will");
    };
    this.getMiss = function()
    {
        return qsTr("Flying");
    };
    this.getCODescription = function()
    {
        return qsTr("Her ground units are very strong. Her other units have normal power.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Increases vision range by 1 and allows her to view into hidden places. Also increases the power of her ground units.");
    };
    this.getPowerName = function()
    {
        return qsTr("Enhanced Vision");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Increases vision range by 2 and allows her to view into hidden places. Also increases the power of her ground units.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Night Vision");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Begin search and destroy operations. Bring the fight to them."),
                qsTr("It's not about firepower. The force with the most information wins."),
                qsTr("I see enemies in the vicinity."),
                qsTr("All units move out. Discipline must be maintain")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Now we have some breathing space..."),
                qsTr("I predicted this outcome...")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("We had not enough intel."),
                qsTr("Next time i am better prepared.")];
    };
    this.getName = function()
    {
        return qsTr("Lin");
    };
}

Constructor.prototype = CO;
var CO_LIN = new Constructor();
