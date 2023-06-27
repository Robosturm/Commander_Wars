var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(2);
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
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
                audio.addMusic("resources/music/cos/koal.mp3", 14166, 103617);
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
            animation.setSound("power0.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BG";
    };

    this.superPowerMovementBonus = 2;
    this.superPowerOffBonus = 70;

    this.powerMovementBonus = 1;
    this.powerOffBonus = 50;
    this.powerDefBonus = 10;
    this.powerBaseOffBonus = 10;

    this.d2dOffBonus = 0;

    this.d2dCoZoneOffBonus = 50;
    this.d2dCoZoneBaseOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (map !== null)
            {
                if (map.onMap(atkPosX, atkPosY))
                {
                    var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
                    var isStreet = (terrainID === "STREET") ||
                            (terrainID === "STREET1") ||
                            (terrainID === "SNOW_STREET") ||
                            (terrainID === "BRIDGE") ||
                            (terrainID === "BRIDGE1") ||
                            (terrainID === "BRIDGE2") ||
                            (terrainID === "WASTE_PATH") ||
                            (terrainID === "DESERT_PATH") ||
                            (terrainID === "DESERT_PATH1");
                    switch (co.getPowerMode())
                    {
                    case GameEnums.PowerMode_Tagpower:
                    case GameEnums.PowerMode_Superpower:
                        if (isStreet)
                        {
                            return CO_KOAL.superPowerOffBonus;
                        }
                        return CO_KOAL.powerBaseOffBonus;
                    case GameEnums.PowerMode_Power:
                        if (isStreet)
                        {
                            return CO_KOAL.powerOffBonus;
                        }
                        return CO_KOAL.powerBaseOffBonus;
                    default:
                        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                        {
                            if (isStreet)
                            {
                                return CO_KOAL.d2dCoZoneOffBonus;
                            }
                            return CO_KOAL.d2dCoZoneBaseOffBonus;
                        }
                        else if (isStreet)
                        {
                            return CO_KOAL.d2dOffBonus;
                        }
                        break;
                    }
                }
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_KOAL.d2dCoZoneBaseOffBonus;
            }
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
                return CO_KOAL.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_KOAL.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {        
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                    co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return CO_KOAL.superPowerMovementBonus;
            }
            else if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {
                return CO_KOAL.powerMovementBonus;
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
                BUILDING.isHq(building))
            {
                return ["ZCOUNIT_HOT_TANK"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A commander of the Black Hole army who is always planning his next destructive act.Endless March");
    };
    this.getHits = function(co)
    {
        return qsTr("Proverbs");
    };
    this.getMiss = function(co)
    {
        return qsTr("Fondue");
    };
    this.getCODescription = function(co)
    {
        return qsTr("A master of road-based battles. Firepower of all units increased on roads.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nHot Tank\n") +
               qsTr("\nGlobal Effect: \nUnits gain increased %0% firepower on street.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain increased %1% firepower on street.");
        text = replaceTextArgs(text, [CO_KOAL.d2dOffBonus, CO_KOAL.d2dCoZoneOffBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Movement range for all units is increased by %0 space. Units have %1% more firepower on roads.");
        text = replaceTextArgs(text, [CO_KOAL.powerMovementBonus, CO_KOAL.powerOffBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Forced March");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Movement range for all units is increased by %0 space. Units have %1% more firepower on roads.");
        text = replaceTextArgs(text, [CO_KOAL.superPowerMovementBonus, CO_KOAL.superPowerOffBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Trail of Woe");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I will crush your units, one by one!"),
                qsTr("I am a warrior and a scholar. My victory is all but certain."),
                qsTr("Heh heh heh. I'll not surrender! Bend your knees and beg for mercy!"),
                qsTr("Heh heh heh... None shall escape."),
                qsTr("My speed knows no equal!"),
                qsTr("Heh heh heh... Fate has smiled on me this day!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I have no equal on the field of war!"),
                qsTr("Wallow in your shame, swine! Wallow, I say"),
                qsTr("Victory is no accident.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("He! He! He!...??? What?"),
                qsTr("One time you win and another time you loose.")];
    };
    this.getName = function()
    {
        return qsTr("Koal");
    };
}

Constructor.prototype = CO;
var CO_KOAL = new Constructor();
