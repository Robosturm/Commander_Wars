var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(5);
        co.setSuperpowerStars(1);
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power_awdc.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_awdc.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/forsythe.mp3", 2245, 118383)
                break;
            }
        }
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
                animation.setSound("power7_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power7_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
            if (animations.length < 5)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power7_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power7_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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

    this.getCOUnitRange = function(co, map)
    {
        return 5;
    };
    this.getCOArmy = function()
    {
        return "BD";
    };

    this.superPowerOffBonus = 30;
    this.superPowerDefBonus = 30;

    this.powerOffBonus = 20;
    this.powerDefBonus = 20;

    this.d2dOffBonus = 0;
    this.d2dDefBonus = 0;

    this.d2dCoZoneOffBonus = 20;
    this.d2dCoZoneDefBonus = 20;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_FORSYTHE.superPowerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_FORSYTHE.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_FORSYTHE.d2dCoZoneOffBonus;
                }
                return CO_FORSYTHE.d2dOffBonus;
            }
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_FORSYTHE.superPowerDefBonus;
            case GameEnums.PowerMode_Power:
                return CO_FORSYTHE.powerDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_FORSYTHE.d2dCoZoneDefBonus;
                }
                return CO_FORSYTHE.d2dDefBonus;
            }
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
                    buildingId === "HQ" ||
                    buildingId === "FORTHQ")
            {
                return ["ZCOUNIT_CHAPERON"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Brown Desert Commander who was called out of retirement to defend his nation. Wants Brown Desert to be free, to live in peace.");
    };
    this.getHits = function(co)
    {
        return qsTr("Honor");
    };
    this.getMiss = function(co)
    {
        return qsTr("Killers");
    };
    this.getCODescription = function(co)
    {
        return qsTr("He has a large CO-Zone which boosts both offensive and defensive bonus.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nChaperon\n") +
               qsTr("\nGlobal Effect: \nUnits have %0% offensive and %1% defensive bonus.") +
               qsTr("\n\nCO Zone Effect: \nUnits have %2% offensive and %3% defensive bonus.");
        text = replaceTextArgs(text, [CO_FORSYTHE.d2dOffBonus, CO_FORSYTHE.d2dDefBonus,
                                      CO_FORSYTHE.d2dCoZoneOffBonus, CO_FORSYTHE.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Increases offensive by %0% and defensive by %1%.");
        text = replaceTextArgs(text, [CO_FORSYTHE.powerOffBonus, CO_FORSYTHE.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Power of Honor");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Increases offensive by %0% and defensive by %1%.");
        text = replaceTextArgs(text, [CO_FORSYTHE.superPowerOffBonus, CO_FORSYTHE.superPowerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Knights of the Desert");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I am a soldier not a killer."),
                qsTr("We fight here to free our people."),
                qsTr("Even in war there are rules, which should not be broken...")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I have beaten you 20 years ago and I beaten you now."),
                qsTr("I am a soldier not a killer."),
                qsTr("Death before dishonour.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("We'll fought young man."),
                qsTr("Great fight. Take your victory with honor.")];
    };
    this.getName = function()
    {
        return qsTr("Forsythe");
    };
}

Constructor.prototype = CO;
var CO_FORSYTHE = new Constructor();
