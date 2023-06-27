var Constructor = function()
{
    this.getCOStyles = function()
    {
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
        if (CO.isActive(co))
        {
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

    this.superPowerOffBonus = 90;

    this.powerOffBonus = 60;
    this.powerDefBonus = 0;

    this.d2dCoZoneOffBonus = 50;
    this.d2dCoZoneDefBonus = 0;

    this.d2dOffBonus = 30;
    this.d2dDefBonus = -10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_GRIMM.superPowerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_GRIMM.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_GRIMM.d2dCoZoneOffBonus;
                }
                break;
            }
            return CO_GRIMM.d2dOffBonus;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_GRIMM.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_GRIMM.d2dCoZoneDefBonus;
            }
            return CO_GRIMM.d2dDefBonus;
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    this.getCOUnits = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            var buildingId = building.getBuildingID();
            if (buildingId === "FACTORY" ||
                buildingId === "TOWN" ||
                BUILDING.isHq(building))
            {
                return ["ZCOUNIT_AT_CYCLE"];
            }
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
        var text = qsTr("\nSpecial Unit:\nAT Cycle\n") +
               qsTr("\nGlobal Effect: \nUnits have %0% reduced defense and %1% increased firepower.") +
               qsTr("\n\nCO Zone Effect: \nUnits have %2% increased firepower bonus.");
        text = replaceTextArgs(text, [CO_GRIMM.d2dDefBonus, CO_GRIMM.d2dOffBonus, CO_GRIMM.d2dCoZoneOffBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Increases the attack of all units by %0%.");
        text = replaceTextArgs(text, [CO_GRIMM.powerOffBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Knuckleduster");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Greatly increases the attack of all units by %0%.");
        text = replaceTextArgs(text, [CO_GRIMM.superPowerOffBonus]);
        return text;
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
