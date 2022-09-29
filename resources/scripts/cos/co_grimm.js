var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
        return ["+alt"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.activatePower = function(co, map)
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

            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power2_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power2_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.activateSuperpower = function(co, powerMode, map)
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

            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 7)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power12_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power12_2.wav", 1, delay);
            }
            if (animations.length < 7)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.loadCOMusic = function(co, map)
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
                audio.addMusic("resources/music/cos/grimm.mp3", 12479, 76493);
                break;
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "YC";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 90;
            case GameEnums.PowerMode_Power:
                return 60;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 50;
                }
                break;
        }
        return 30;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) ||
            co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 0;
        }
        return -10;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_AT_CYCLE"];
        }
        return [];
    };


    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A Yellow Comet commander with a dynamic personality. Could care less about the details. His nickname is Lighting Grimm.");
    };
    this.getHits = function(co)
    {
        return qsTr("Donuts");
    };
    this.getMiss = function(co)
    {
        return qsTr("Planning");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Firepower of all units is increased, thanks to his daredevil nature, but their defenses are reduced.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nAT Cycle\n") +
               qsTr("\nGlobal Effect: \nUnits have reduced defense and increased firepower.") +
               qsTr("\n\nCO Zone Effect: \nUnits have high offensive bonus and a defensive malus.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Increases the attack of all units.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Knuckleduster");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Greatly increases the attack of all units.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Haymaker");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Things are lookin' Grimm for you! Harrrrr!"),
                qsTr("You're about to enter a world of pain!!"),
                qsTr("Outta the way! I got crushin' to do!"),
                qsTr("Oooh, yeah!!|Gwar har har!! Go cry like a little girl!!"),
                qsTr("What a pencil neck!!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Wanna throw down again? Oooh yeah!"),
                qsTr("Gwar har har! Hit the road, slick!"),
                qsTr("Fear the lightning!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I'm tellin' you, this is awful!"),
                qsTr("I'll get you next time! Oooh yeah!")];
    };
    this.getName = function()
    {
        return qsTr("Grimm");
    };
}

Constructor.prototype = CO;
var CO_GRIMM = new Constructor();
