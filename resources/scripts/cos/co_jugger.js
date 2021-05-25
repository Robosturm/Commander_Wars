var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
    };

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
                audio.addMusic("resources/music/cos/bh_power.mp3", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/jugger.mp3", 7861, 81685);
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
                animation.setSound("power7_1.wav");
            }
            else
            {
                animation.setSound("power7_2.wav");
            }
            if (animations.length < 5)
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

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
                animation.setSound("power12_1.wav");
            }
            else
            {
                animation.setSound("power12_2.wav");
            }
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
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BG";
    };
    this.getBonusLuck = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 95;
            case GameEnums.PowerMode_Power:
                return 55;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return 30;
                }
                break;
        }
        return 14;
    };
	
    this.getBonusMisfortune = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 45;
            case GameEnums.PowerMode_Power:
                return 25;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return 15;
                }
                break;
        }
        return 7;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
    {
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
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
            return ["ZCOUNIT_AUTO_TANK"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A robot-like commander in the Bolt Guard army. No one knows his true identity!");
    };
    this.getHits = function(co)
    {
        return qsTr("Energy");
    };
    this.getMiss = function(co)
    {
        return qsTr("Static electricity");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Units may suddenly deal more damage than expected, but their firepower is inherently low.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nAuto Tank\n") +
               qsTr("\nGlobal Effect: \nUnits have more Luck and Misfortune") +
               qsTr("\n\nCO Zone Effect: \nUnits have even more Luck and Misfortune");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("There is a chance he might get a stronger blow, but base firepower is slightly reduced.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Overclock");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("There is a chance he might get a devestating blow, but firepower is reduced.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("System Crash");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Enemy: Prepare for mega hurtz."),
                qsTr("Memory: upgraded. Shell: shined. Ready to uh...roll."),
                qsTr("Enemy system purge initiated..."),
                qsTr("Blue screen of death!"),
                qsTr("Crushware loaded..."),
                qsTr("Approaching system meltdown.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Victory; downloading party hat."),
                qsTr("Victory dance initiated."),
                qsTr("Jugger; superior. Enemy; lame.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Critical Error: Does not compute."),
                qsTr("Victory impossible! Units overwhelmed. Jugger must... Control-Alt-Delete.")];
    };
    this.getName = function()
    {
        return qsTr("Jugger");
    };
}

Constructor.prototype = CO;
var CO_JUGGER = new Constructor();
